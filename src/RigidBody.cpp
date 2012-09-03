#include <Physics/RigidBody.h>
#include <Physics/CollisionShape.h>
#include <btBulletDynamicsCommon.h>

using namespace Uberngine;

void RigidBody::SetMass(float mass) {
  btVector3 Inertia(0,0,0);
  if (mass > 0.0f)
    Body->getCollisionShape()->calculateLocalInertia(mass, Inertia);
  Body->setMassProps(mass, Inertia);
}

void RigidBody::SetKinematic(bool kin) {
  Kinematic = kin;
  if (kin) {
    Body->setCollisionFlags(Body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    Body->setMotionState(&KinematicMS);
    Body->setActivationState(DISABLE_DEACTIVATION);
  } else {
    Body->setCollisionFlags(Body->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
    Body->setMotionState(&NormalMS);
    //Body->forceActivationState(ACTIVE_TAG);
    Body->activate(true);
  }
}

void RigidBody::SetPosition(const float *pos) {
  Transform.setFromOpenGLMatrix(pos);
}

void RigidBody::GetPosition(float *pos) {
  Transform.getOpenGLMatrix(pos);
}

RigidBody::RigidBody(float mass, CollisionShape *shape) : Kinematic(false), KinematicMS(Transform), NormalMS(Transform) {

  btVector3 Inertia(0,0,0);
  Transform.setIdentity();
  if (mass > 0.0f) {
    shape->Shape->calculateLocalInertia(mass, Inertia);
  }
  btRigidBody::btRigidBodyConstructionInfo RigidBodyInfo(mass, &NormalMS, shape->Shape, Inertia);
  Body = new btRigidBody(RigidBodyInfo);
}

RigidBody::~RigidBody() { delete Body; }
