#include <Physics/CollisionShape.h>

using namespace Uberngine;

CollisionShape *CollisionShape::CreateBoxShape(float x, float y, float z) {

  CollisionShape *NewShape = new CollisionShape;
  NewShape->Shape = new btBoxShape(btVector3(x, y, z));

  return NewShape;
}

CollisionShape *CollisionShape::CreateSphereShape(float radius) {
  CollisionShape *NewShape = new CollisionShape;
  NewShape->Shape = new btSphereShape(radius);

  return NewShape;
}
