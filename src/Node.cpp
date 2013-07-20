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

PureNode::PureNode() : GLTransform(1.0f), 
                       Body(nullptr) {}

PureNode::~PureNode() {

//  SetMesh(nullptr);
  SetRigidBody(nullptr);
}

void PureNode::SetRigidBody(RigidBody* rb) {
  if (Body != nullptr) {
    delete Body;
  }
  Body = rb;
}

void PureNode::SetTransform(float transX, float transY, float transZ, float angle, float axisX, 
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

