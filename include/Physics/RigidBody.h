#ifndef __UBERNGINE_RIGIDBODY_H__
#define __UBERNGINE_RIGIDBODY_H__

#include <btBulletDynamicsCommon.h>

namespace Uberngine {

class DynamicsWorld;
class CollisionShape;

class RigidBody {

  class KMotionState : public btMotionState {
    btTransform &CurrentPosition;
  public:
    KMotionState(btTransform &pos) : CurrentPosition(pos) {}
    virtual void getWorldTransform(btTransform &worldTrans) const { worldTrans = CurrentPosition; }
    void setPosition(btTransform &current) { CurrentPosition = current; }
    virtual void setWorldTransform(const btTransform &worldTrans) { }
  };

  class MotionState : public btMotionState {
    btTransform &CurrentPosition;
  public:
    MotionState(btTransform &pos) : CurrentPosition(pos) {}
    virtual void getWorldTransform(btTransform &worldTrans) const { worldTrans = CurrentPosition; }
    virtual void setWorldTransform(const btTransform &worldTrans) { CurrentPosition = worldTrans; }
  };

  btRigidBody *Body;
  btTransform Transform;
  KMotionState KinematicMS;
  MotionState NormalMS;
  bool Kinematic;
  friend class DynamicsWorld;
public:
  void SetMass(float mass);
  void SetKinematic(bool kin);
  bool IsKinematic() { return Kinematic; }
  void SetPosition(const float *pos);
  void GetPosition(float *pos);
  int GetActivationState() { return Body->getActivationState(); }
  RigidBody(float mass, CollisionShape *shape);
  ~RigidBody();
};

}

#endif
