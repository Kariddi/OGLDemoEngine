#ifndef __UBERNGINE_COLLISIONSHAPE_H__
#define __UBERNGINE_COLLISIONSHAPE_H__

#include <btBulletDynamicsCommon.h>

namespace Uberngine {

class RigidBody;

class CollisionShape {
  btCollisionShape *Shape;
  CollisionShape() : Shape(NULL) {}
  friend class RigidBody;
public:
  btCollisionShape *GetBulletObject() { return Shape; }
  static CollisionShape *CreateBoxShape(float x, float y, float z);
  static CollisionShape *CreateSphereShape(float radius);
  ~CollisionShape() { delete Shape; }
};

}

#endif
