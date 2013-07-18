#ifndef __UBERNGINE_DYNAMICSWORLD_H__
#define __UBERNGINE_DYNAMICSWORLD_H__

#include <Physics/RigidBody.h>
#include <btBulletDynamicsCommon.h>

namespace Uberngine {

class DynamicsWorld {
  btDynamicsWorld *World;
  friend class PhysicsManager;
  DynamicsWorld() : World(nullptr) {}
  ~DynamicsWorld() { delete World; }
public:
  void AddRigidBody(RigidBody *rb) { World->addRigidBody(rb->Body); }
  void RemoveRigidBody(RigidBody *rb) { World->removeRigidBody(rb->Body); }
  int GetNumObjects() { return World->getNumCollisionObjects(); }
  void SynchronizeMotionStates() { World->synchronizeMotionStates(); }
  void StepWorld(float time_step, int max_sub, float fixed_time_step = (1.0f/60.0f)) { World->stepSimulation(time_step, max_sub, fixed_time_step); }
  void SetGravity(float x, float y, float z) {
    World->setGravity(btVector3(x, y, z));
  }
};

}

#endif
