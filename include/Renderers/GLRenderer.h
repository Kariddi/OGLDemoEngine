#ifndef __UBERNGINE_GLRENDERER_H__
#define __UBERNGINE_GLRENDERER_H__

#include <sstream>
#include <glm/glm.hpp> //vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale,
#include <glm/gtc/matrix_inverse.hpp>
#include <Renderers/Renderer.h>
#include <OGL.h>

namespace Uberngine {

class GLRenderer : public Renderer<GLRenderer> {
  struct PartRenderCtx {
    GLuint PartVAO;
    GLuint VBO;
  };

  Mesh* NodeMesh;
  PartRenderCtx* RenderCTXs;
  GLuint VertVBO;
  GLuint* Textures;
  //GLuint Samplers[3];
  Shader* ShaderProg;
  GLenum* GLIdxType;
  GLenum GLNormType;
  GLenum GLTexType;
  
public:

  GLRenderer() : NodeMesh(nullptr), RenderCTXs(nullptr), ShaderProg(nullptr), 
    GLIdxType(nullptr), GLNormType(GL_FLOAT), GLTexType(GL_FLOAT) {}
  
  void Render(const EngineReal* view_mat, const EngineReal* proj_mat, 
  	const glm::mat4& comulative, const LightList& lights) {
//  std::cout << "Rendering Node" << std::endl;
  //If the node doesn't have an associated mesh or the shader is invalid don't render
  if (NodeMesh && ShaderProg && ShaderProg->isValid()) {
    GLuint Program = ShaderProg->getProgram();
    //Setting the shader program
    glUseProgram(ShaderProg->getProgram());
    //Setting the View transform matrix
    GLuint Loc = glGetUniformLocation(Program, "V");
    glUniformMatrix4fv(Loc, 1, GL_FALSE, view_mat);
   //Setting the Projection matrix
    Loc = glGetUniformLocation(Program, "P");
    
    glUniformMatrix4fv(Loc, 1, GL_FALSE, proj_mat);
    //Setting the Model matrix
    Loc = glGetUniformLocation(Program, "M");
    glUniformMatrix4fv(Loc, 1, GL_FALSE, glm::value_ptr(comulative));
    //Setting the Inverse transform matrix for normal transformation
    Loc = glGetUniformLocation(Program, "MVN");
    glm::mat4 View = glm::make_mat4(view_mat);
    glm::mat4 InvTView = View*comulative;
    InvTView = glm::inverseTranspose(InvTView);
    glUniformMatrix4fv(Loc, 1, GL_FALSE, glm::value_ptr(InvTView));

    const LightList &LL = lights; 
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
      glUniform1i(Loc, static_cast<GLint>(LL[i]->GetLightType()));
    }
    //Set if lighting is being used
    Loc = glGetUniformLocation(Program, "is_lighted");
    if (light_num > 0)
      glUniform1i(Loc, 1);
    else
      glUniform1i(Loc, 0);

    int i = 0;
    //For each part of the mesh
    //for (Mesh::PartListIt I = NodeMesh->Parts.begin(), E = NodeMesh->Parts.end(); I != E; ++I) {
    for (auto I : NodeMesh->Parts) {
      //Set the material data
      //GLuint VAO = RenderCTXs[i].PartVAO;
      //int Ind = (*I)->IndicesSize;
      Loc = glGetUniformLocation(Program, "Mat.ambient");
      glUniform3f(Loc, I->Mat.KAr, I->Mat.KAg, I->Mat.KAb);
      Loc = glGetUniformLocation(Program, "Mat.diffuse");
      glUniform3f(Loc, I->Mat.KDr, I->Mat.KDg, I->Mat.KDb);
      Loc = glGetUniformLocation(Program, "Mat.specular");
      glUniform3f(Loc, I->Mat.KSr, I->Mat.KSg, I->Mat.KSb);
      Loc = glGetUniformLocation(Program, "Mat.spec_c");
      glUniform1f(Loc, I->Mat.Ns);
/*      //Bind samplers
      for (int i = 0; i < 3; ++i)
        glBindSampler(i, Samplers[i]);
*/
      //Set textures
      int tex_num = I->Mat.TextureIdx.size();
      for (int i2 = 0; i2 < tex_num; ++i2) {
        std::stringstream ss;
        ss << "Textures[" << i2 << "]";
        Loc = glGetUniformLocation(Program, ss.str().c_str());
        glUniform1i(Loc, i2);
        glActiveTexture(GL_TEXTURE0 + i2);
        glBindTexture(GL_TEXTURE_2D, Textures[I->Mat.TextureIdx[i2]]);
      }

      //Bind the vertex array and draw
      glBindVertexArray(RenderCTXs[i].PartVAO);
      glDrawElements(GL_TRIANGLES, I->IndicesSize, GLIdxType[i], (void*) 0);
      ++i;
    }
    glBindVertexArray(0);
  }

  //Render the sub-nodes
//  for (NodeListIt I = Children.begin(), E = Children.end(); I != E; ++I)
//    (*I)->Render(scene);
  }

  void SetMesh(Mesh* mesh) {
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
      delete [] GLIdxType;
      glDeleteBuffers(1, &VertVBO);
    }

    if (mesh == nullptr)
      return;

    if (mesh->AttInfo.NormSize == 4) {
      GLNormType = GL_FLOAT;
    } else if (mesh->AttInfo.NormSize == 2) {
      if (mesh->AttInfo.NormSigned) {
        GLNormType = GL_SHORT;
      } else {
        GLNormType = GL_UNSIGNED_SHORT;
      }
    } else if (mesh->AttInfo.NormSize == 1) {
      if (mesh->AttInfo.NormSigned) {
        GLNormType = GL_BYTE;
      } else {
        GLNormType = GL_UNSIGNED_BYTE;
      }
    }

    if (mesh->AttInfo.TexSize == 4) {
      GLTexType = GL_FLOAT;
    } else if (mesh->AttInfo.TexSize == 2) {
      if (mesh->AttInfo.TexSigned) {
        GLTexType = GL_SHORT;
      } else {
        GLTexType = GL_UNSIGNED_SHORT;
      }
    } else if (mesh->AttInfo.TexSize == 1) {
      if (mesh->AttInfo.TexSigned) {
        GLTexType = GL_BYTE;
      } else {
        GLTexType = GL_UNSIGNED_BYTE;
      }
    }
    
    int num_parts = mesh->Parts.size();
    GLIdxType = new GLenum[num_parts];

    //Create textures
    int tex_num = mesh->Textures.size();
    if (tex_num > 0) {
      Textures = new GLuint[tex_num];
      glGenTextures(mesh->Textures.size(), Textures);
      for (int i = 0; i < tex_num; ++i) {
        Texture &tex = *mesh->Textures[i];
        glBindTexture(GL_TEXTURE_2D, Textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, tex.format,  tex.width, 
                     tex.height, 0, tex.format, tex.type, 
                     tex.imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
      }
      glBindTexture(GL_TEXTURE_2D, 0);
    }

    //Create and prepare the VBOs
    glGenBuffers(1, &VertVBO);
    glBindBuffer(GL_ARRAY_BUFFER, VertVBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->VerticesNum*mesh->AttInfo.VertexStride, 
                 mesh->Vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    RenderCTXs = new PartRenderCtx[num_parts];
    //For each mesh part create the VAOs
    int i = 0;
    for (Mesh::PartListIt I = mesh->Parts.begin(), E = mesh->Parts.end(); I != E; ++I) {
      glGenVertexArrays(1, &RenderCTXs[i].PartVAO);
      glBindVertexArray(RenderCTXs[i].PartVAO);
      glBindBuffer(GL_ARRAY_BUFFER, VertVBO);
      glEnableVertexAttribArray(0);
      if (mesh->AttInfo.HasTexture)
        glEnableVertexAttribArray(1);
      if (mesh->AttInfo.HasNormals)
        glEnableVertexAttribArray(2);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mesh->AttInfo.VertexStride, reinterpret_cast<void*>(0));
      if (mesh->AttInfo.HasTexture) {
        GLboolean normalized = (GLTexType != GL_FLOAT && mesh->AttInfo.TexSigned) ? GL_TRUE : GL_FALSE;
        glVertexAttribPointer(1, 2, GLTexType, normalized, mesh->AttInfo.VertexStride, reinterpret_cast<void*>(mesh->AttInfo.TexOffset));
      }
      if (mesh->AttInfo.HasNormals) {
        GLboolean normalized = (GLNormType != GL_FLOAT && mesh->AttInfo.NormSigned) ? GL_TRUE : GL_FALSE;
        glVertexAttribPointer(2, 3, GLNormType, normalized, mesh->AttInfo.VertexStride, 
                              reinterpret_cast<void*>(mesh->AttInfo.NormOffset));
      }
      glGenBuffers(1, &RenderCTXs[i].VBO);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderCTXs[i].VBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, (*I)->IndicesSize*(*I)->IdxElementSize, 
                   (*I)->Indices, GL_STATIC_DRAW);

      glBindVertexArray(0);
      for (int j = 0; j < 3; ++j)
        glDisableVertexAttribArray(j);
      
      if ((*I)->IdxElementSize == 4)
        GLIdxType[i] = GL_UNSIGNED_INT;
      else if ((*I)->IdxElementSize == 2)
        GLIdxType[i] = GL_UNSIGNED_SHORT;
      else if ((*I)->IdxElementSize == 1)
        GLIdxType[i] = GL_UNSIGNED_BYTE;

      ++i;
    }
    NodeMesh = mesh;
  }

  void SetShader(Shader* shader) {
    if (ShaderProg)
      delete ShaderProg;
    ShaderProg = shader;
  }

//  void SetEffect(Effect* effect) {
//    static_cast<Derived*>(this)->SetEffect(effect);
//  }
};

}

#endif
