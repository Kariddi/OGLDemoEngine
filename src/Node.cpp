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
               GLTransform(1.0f) {}

Node::~Node() {

  SetMesh(nullptr);
  SetRigidBody(nullptr);
}

void Node::GlobalInitialize() {
  //Initialize();
  for (auto I : Children)
    I->GlobalInitialize();
}

void Node::GlobalUpdate() {
  //std::cout << "Global Updating Node" << std::endl;
  //Update();
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
  if (Body != nullptr) {
    delete Body;
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

