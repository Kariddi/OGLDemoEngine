#include <Physics/PhysicsManager.h>
#include <Physics/CollisionShape.h>
#include <Physics/DynamicsWorld.h>
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

  for (WorldSetTyIt I = WorldSet.begin(), E = WorldSet.end(); I != E; ++I)
    delete *I;
}

CollisionShape *PhysicsManager::GetBoxShape(float x_size, float y_size, float z_size) {
  btVector3 BoxSize(x_size, y_size, z_size);
  BoxMapTyIt BoxIt = BoxMap.find(BoxSize);
  if (BoxIt != BoxMap.end())
    return BoxIt->second;

  CollisionShape *NewBox = CollisionShape::CreateBoxShape(x_size, y_size, z_size);
  BoxMap[BoxSize] = NewBox;

  return NewBox;
}

CollisionShape *PhysicsManager::GetSphereShape(float radius) {
  SphereMapTyIt SphereIt = SphereMap.find(radius);
  if (SphereIt != SphereMap.end())
    return SphereIt->second;

  CollisionShape *NewSphere = CollisionShape::CreateSphereShape(radius);
  SphereMap[radius] = NewSphere;

  return NewSphere;
}

DynamicsWorld *PhysicsManager::GetNewDynamicsWorld() {

  DynamicsWorld *NewWorld = new DynamicsWorld;
  NewWorld->World = new btDiscreteDynamicsWorld(CDispatcher, Broadphase, ConstraintSolver, CollisionC);
  WorldSet.insert(NewWorld);

  return NewWorld;
}

void PhysicsManager::DisposeDynamicsWorld(DynamicsWorld *world) {
  WorldSetTyIt It = WorldSet.find(world);
  if (It != WorldSet.end()) {
    WorldSet.erase(It);
    delete world;
  }
}
