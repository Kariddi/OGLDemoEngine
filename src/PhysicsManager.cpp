#include <PhysicsManager.h>
#include <btBulletDynamicsCommon.h>

using namespace Uberngine;

PhysicsManager::PhysicsManager() {
  CollisionC = new btDefaultCollisionConfiguration;
  CDispatcher = new btCollisionDispatcher(CollisionC);
  Broadphase = new btDbvtBroadphase;
  ConstraintSolver = new btSequentialImpulseConstraintSolver;
}

PhysicsManager::~PhysicsManager() {
  delete ConstraintSolver;
  delete Broadphase;
  delete CDispatcher;
  delete CollisionC;
  for (BoxMapTyIt I = BoxMap.begin(), E = BoxMap.end(); I != E; ++I)
    delete I->second;
  
  for (SphereMapTyIt I = SphereMap.begin(), E = SphereMap.end(); I != E; ++I)
    delete I->second;
}

btCollisionShape *PhysicsManager::GetBoxShape(float x_size, float y_size, float z_size) {
  btVector3 BoxSize(x_size, y_size, z_size);
  BoxMapTyIt BoxIt = BoxMap.find(BoxSize);
  if (BoxIt != BoxMap.end())
    return BoxIt->second;

  btBoxShape *NewBox = new btBoxShape(BoxSize);
  BoxMap[BoxSize] = NewBox;

  return NewBox;
}

btCollisionShape *PhysicsManager::GetSphereShape(float radius) {
  SphereMapTyIt SphereIt = SphereMap.find(radius);
  if (SphereIt != SphereMap.end())
    return SphereIt->second;

  btSphereShape *NewSphere = new btSphereShape(radius);
  SphereMap[radius] = NewSphere;

  return NewSphere;
}
