#include <Node.h>
#include <Camera.h>
#include <Scene.h>
#include <Shader.h>
#include <Constants.h>
#include <Physics/DynamicsWorld.h>
#include <Engine.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <glm/gtc/matrix_inverse.hpp>
#include <Defs.h>

using namespace Uberngine;

Node::Node() : Parent(nullptr), CurrentScene(nullptr), Body(nullptr),
               GLTransform(1.0f) {
  //Rend = RendererFactory<OpenGL>::BuildNewRenderer();
}

Node::Node(Node* parent) : Node()
{
  if (parent != nullptr) {
    this->CurrentScene = parent->CurrentScene;
    parent->AddChildNode(this); 
  }
}

Node::Node(Scene* parent) : Node()
{
  if (parent != nullptr)
    this->CurrentScene = parent; 
}

Node::~Node() {
  for (NodeListIt I = Children.begin(), E = Children.end(); I != E; ++I)
    delete *I;

  SetMesh(nullptr);
  //if (DynWorld != nullptr)
  SetRigidBody(nullptr);
  //delete Rend;
}

void Node::Render(Scene* scene) {

  Camera* Cam = scene->GetCamera();

  Rend.Render(Cam->GetViewMatrix(), Cam->GetProjMatrix(), 
    Comulative,scene->GetLights());

  //Render the sub-nodes
  for (auto I : Children)
    I->Render(scene);
}

void Node::GlobalInitialize() {
  Initialize();
  for (auto I : Children)
    I->GlobalInitialize();
}

void Node::GlobalUpdate() {
  //std::cout << "Global Updating Node" << std::endl;
  Update();
  if (Body != nullptr) {
    float trans[16];
    Body->GetPosition(trans);
    Comulative = glm::make_mat4(trans);
  } else {    
  //Multiplying the Model matrix with the Comulative matrix of the parent
  //Child nodes move together with parent nodes (if the parent node moves the child node moves with him)
    if (Parent)
      Comulative = Parent->Comulative*glm::make_mat4(GetTransform());
    else
      Comulative = glm::mat4(1.0f);
  }

  for (auto I : Children)
    I->GlobalUpdate();
}

void Node::AddChildNode(Node* node) {
  if (node->Parent != nullptr) {
    NodeList &OldPChildren = node->Parent->Children;
    OldPChildren.erase(std::find(OldPChildren.begin(), OldPChildren.end(), node));
  }
  node->Parent = this;
  //node->DynWorld = nullptr;
  Children.push_back(node);
}

void Node::SetMesh(Mesh* mesh) {
  Rend.SetMesh(mesh);
}

void Node::SetShader(Shader* shader) {
  Rend.SetShader(shader);
}

void Node::SetRigidBody(RigidBody* rb) {
  DynamicsWorld& DWorld = *CurrentScene->GetDynamicsWorld();
  if (Body != nullptr) {
    DWorld.RemoveRigidBody(Body);
    delete Body;
  }
  if (rb != nullptr) {
    rb->SetKinematic(true);
    rb->SetPosition(GetTransform());
    rb->SetKinematic(false);
    DWorld.AddRigidBody(rb);
  }
  Body = rb;
}

void Node::SetTransform(float transX, float transY, float transZ, float angle, float axisX, 
                        float axisY, float axisZ) {
  if (!IsKinematic())
    return;
  float sint = sinf(angle/2);
  GLTransform = glm::translate(glm::mat4(), glm::vec3(transX, transY, transZ));
  glm::quat Rot(cosf(angle/2), axisX*sint, axisY*sint, axisZ*sint);
  GLTransform *= glm::mat4_cast(glm::normalize(Rot));
  if (Body)
    Body->SetPosition(glm::value_ptr(GLTransform));
}

