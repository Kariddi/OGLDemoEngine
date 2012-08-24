#include <Node.h>
#include <Camera.h>
#include <Scene.h>
#include <Shader.h>
#include <Constants.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <glm/gtc/matrix_inverse.hpp>
#include <Defs.h>

using namespace Uberngine;

Node::Node(BaseEngine *e, Node *parent) : NodeMesh(NULL), RenderCTXs(NULL), ShaderProg(NULL), Parent(NULL),
                                            GLTransform(1.0f), Eng(*e) {
 if (parent != NULL)
   parent->AddChildNode(this); 
}

Node::~Node() {
  for (NodeListIt I = Children.begin(), E = Children.end(); I != E; ++I)
    delete *I;
//  _MeshTy *Mesh = NodeMesh;
  SetMesh(NULL);
}

void Node::Render(Scene *scene) {
//  std::cout << "Rendering Node" << std::endl;
  //If the node doesn't have an associated mesh or the shader is invalid don't render
  if (NodeMesh && ShaderProg && ShaderProg->isValid()) {
    GLuint Program = ShaderProg->getProgram();
    //Setting the shader program
    glUseProgram(ShaderProg->getProgram());
    Camera *Cam = scene->GetCamera();
    //Setting the View transform matrix
    GLuint Loc = glGetUniformLocation(Program, "V");
    glUniformMatrix4fv(Loc, 1, GL_FALSE, Cam->GetViewMatrix());
    //Setting the Projection matrix
    Loc = glGetUniformLocation(Program, "P");
    
    glUniformMatrix4fv(Loc, 1, GL_FALSE, Cam->GetProjMatrix());
    
    //Multiplying the Model matrix with the Comulative matrix of the parent
    //Child nodes move together with parent nodes (if the parent node moves the child node moves with him)
    if (Parent)
      Comulative = Parent->Comulative*glm::make_mat4(GetTransform());
    else
      Comulative = glm::mat4(1.0f);

    //Setting the Model matrix
    Loc = glGetUniformLocation(Program, "M");
    glUniformMatrix4fv(Loc, 1, GL_FALSE, glm::value_ptr(Comulative));
    //Setting the Inverse transform matrix for normal transformation
    Loc = glGetUniformLocation(Program, "MVN");
    glm::mat4 View = glm::make_mat4(Cam->GetViewMatrix());
    glm::mat4 InvTView = View*Comulative;
    InvTView = glm::inverseTranspose(InvTView);
    glUniformMatrix4fv(Loc, 1, GL_FALSE, glm::value_ptr(InvTView));

    const Scene::LightList &LL = scene->GetLights(); 
    int light_num = LL.size();
    Loc = glGetUniformLocation(Program, "LightNum");
    glUniform1i(Loc, light_num);
    //Setting the lights parameters
    for (int i = 0; i < light_num; ++i) {
      std::stringstream ss;
      ss << "L[" << i << "]";
      Loc = glGetUniformLocation(Program, (ss.str() + ".position").c_str());
      glm::vec4 Pos = glm::make_vec4(LL[i]->GetPosition());
      glUniform3fv(Loc, 1, glm::value_ptr(Pos*View));
      Loc = glGetUniformLocation(Program, (ss.str() + ".dir").c_str());
      glm::vec4 Dir = glm::make_vec4(LL[i]->GetDirection());
      glUniform3fv(Loc, 1, glm::value_ptr(Dir*View));
      Loc = glGetUniformLocation(Program, (ss.str() + ".color").c_str());
      glUniform3fv(Loc, 1, LL[i]->GetColor());
      Loc = glGetUniformLocation(Program, (ss.str() + ".type").c_str());
      glUniform1i(Loc, LL[i]->GetLightType());
    }
    //Set if lighting is being used
    Loc = glGetUniformLocation(Program, "is_lighted");
    if (light_num > 0)
      glUniform1i(Loc, 1);
    else
      glUniform1i(Loc, 0);

    int i = 0;
    //For each part of the mesh
    for (MeshTy::PartListIt I = NodeMesh->Parts.begin(), E = NodeMesh->Parts.end(); I != E; ++I) {
      //Set the material data
      //GLuint VAO = RenderCTXs[i].PartVAO;
      //int Ind = (*I)->IndicesSize;
      Loc = glGetUniformLocation(Program, "Mat.ambient");
      glUniform3f(Loc, (*I)->Mat.KAr, (*I)->Mat.KAg, (*I)->Mat.KAb);
      Loc = glGetUniformLocation(Program, "Mat.diffuse");
      glUniform3f(Loc, (*I)->Mat.KDr, (*I)->Mat.KDg, (*I)->Mat.KDb);
      Loc = glGetUniformLocation(Program, "Mat.specular");
      glUniform3f(Loc, (*I)->Mat.KSr, (*I)->Mat.KSg, (*I)->Mat.KSb);
      Loc = glGetUniformLocation(Program, "Mat.spec_c");
      glUniform1f(Loc, (*I)->Mat.Ns);
/*      //Bind samplers
      for (int i = 0; i < 3; ++i)
        glBindSampler(i, Samplers[i]);
*/
      //Set textures
      int tex_num = (*I)->Mat.TextureIdx.size();
      for (int i = 0; i < tex_num; ++i) {
        std::stringstream ss;
        ss << "Textures[" << i << "]";
        Loc = glGetUniformLocation(Program, ss.str().c_str());
        glUniform1i(Loc, i);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, Textures[(*I)->Mat.TextureIdx[i]]);
      }

      //Bind the vertex array and draw
      glBindVertexArray(RenderCTXs[i].PartVAO);
      glDrawElements(GL_TRIANGLES, (*I)->IndicesSize, GL_UNSIGNED_INT, (void*) 0);
      ++i;
    }
    glBindVertexArray(0);
  }

  //Render the sub-nodes
  for (NodeListIt I = Children.begin(), E = Children.end(); I != E; ++I)
    (*I)->Render(scene);
}

void Node::GlobalInitialize() {
  Initialize();
  for (NodeListIt I = Children.begin(), E = Children.end(); I != E; ++I)
    (*I)->GlobalInitialize();
}

void Node::GlobalUpdate() {
  //std::cout << "Global Updating Node" << std::endl;
  Update();
  for (NodeListIt I = Children.begin(), E = Children.end(); I != E; ++I)
    (*I)->GlobalUpdate();
}

void Node::AddChildNode(Node *node) {
  if (node->Parent != NULL) {
    NodeList &OldPChildren = node->Parent->Children;
    OldPChildren.erase(std::find(OldPChildren.begin(), OldPChildren.end(), node));
  }
  node->Parent = this;
  Children.push_back(node);
}

void Node::SetMesh(MeshTy *mesh) {
  //Clear data associated to the previously associated mesh
  if (RenderCTXs) {
    int old_num_parts = NodeMesh->Parts.size();
    int old_tex_nums = NodeMesh->Textures.size();
    for (int i = 0; i < old_num_parts; ++i) {
      glDeleteVertexArrays(1, &RenderCTXs[i].PartVAO);
      glDeleteBuffers(1, &RenderCTXs[i].VBO); 
    }
    glDeleteTextures(old_tex_nums, Textures);
  //  glDeleteSamplers(3, Samplers);
    delete [] RenderCTXs;
    glDeleteBuffers(1, &VertVBO);
  }

  if (mesh == NULL)
    return;

  int num_parts = mesh->Parts.size();
  int stride = 3*sizeof(Traits::MeshVertexType);
  if (mesh->HasTexture)
    stride += 2*sizeof(Traits::MeshVertexType);
  if (mesh->HasNormals)
    stride += 3*sizeof(Traits::MeshVertexType);

  //Create textures
  int tex_num = mesh->Textures.size();
  if (tex_num > 0) {
    Textures = new GLuint[tex_num];
    glGenTextures(mesh->Textures.size(), Textures);
    for (int i = 0; i < tex_num; ++i) {
      glBindTexture(GL_TEXTURE_2D, Textures[i]);
      glTexImage2D(GL_TEXTURE_2D, 0, mesh->Textures[i]->type,  mesh->Textures[i]->width, 
                   mesh->Textures[i]->height, 0, mesh->Textures[i]->type, GL_UNSIGNED_BYTE, 
                   mesh->Textures[i]->imageData);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  //Create and prepare the VBOs
  glGenBuffers(1, &VertVBO);
  glBindBuffer(GL_ARRAY_BUFFER, VertVBO);
  glBufferData(GL_ARRAY_BUFFER, mesh->VerticesNum*stride, 
               mesh->Vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  RenderCTXs = new PartRenderCtx[num_parts];
  //For each mesh part create the VAOs
  int i = 0;
  int offset = sizeof(Traits::MeshVertexType)*3;
  for (_MeshTy::PartListIt I = mesh->Parts.begin(), E = mesh->Parts.end(); I != E; ++I) {
    glGenVertexArrays(1, &RenderCTXs[i].PartVAO);
    glBindVertexArray(RenderCTXs[i].PartVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VertVBO);
	glEnableVertexAttribArray(0);
	if (mesh->HasTexture)
      glEnableVertexAttribArray(1);
	if (mesh->HasNormals)
	  glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GLTraits::VertexDataTy, GL_FALSE, stride, (void*) 0);
    if (mesh->HasTexture) {
      glVertexAttribPointer(1, 2, GLTraits::VertexDataTy, GL_FALSE, stride, (void*)  offset);
      offset += sizeof(Traits::MeshVertexType)*2;
    }
    if (mesh->HasNormals)
      glVertexAttribPointer(2, 3, GLTraits::VertexDataTy, GL_FALSE, stride, (void*) offset);
    glGenBuffers(1, &RenderCTXs[i].VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderCTXs[i].VBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (*I)->IndicesSize*sizeof(Traits::MeshIDXType), 
                 (*I)->Indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    for (int j = 0; j < 3; ++j)
      glDisableVertexAttribArray(j);

    ++i;
  }
  NodeMesh = mesh;
  //glBindVertexArray(0);
}

void Node::SetShader(Shader *shader) {
  if (ShaderProg)
    delete ShaderProg;
  ShaderProg = shader;
}

void Node::SetTransform(float transX, float transY, float transZ, float angle, float axisX, 
                        float axisY, float axisZ) {
  GLTransform = glm::mat4(1.0f);
  float sint = sinf(angle/2);
  GLTransform = glm::translate(GLTransform, glm::vec3(transX, transY, transZ));
  glm::quat Rot(axisX*sint, axisY*sint, axisZ*sint, cosf(angle/2));
  GLTransform *= glm::mat4_cast(glm::normalize(Rot));
}

