#ifndef __UBERNGINE_PHYSICSMANAGER_H__
#define __UBERNGINE_PHYSICSMANAGER_H__

#include <map>
#include <set>
#include <LinearMath/btVector3.h>
#include <btBulletDynamicsCommon.h>

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btBoxShape;
class btSphereShape;
class btCollisionShape;

namespace Uberngine {

class CollisionShape;
class DynamicsWorld;

//template<bool phys_support>
class PhysicsManager {
  struct VectorComparator {
  public:
    bool operator()(const btVector3 &a, const btVector3 &b) {
      if (a.x() < b.x())
        return true;
      if (a.y() < b.y())
        return true;
      if (a.z() < b.z())
        return true;
      return false;
    }
  };

typedef std::map<btVector3, CollisionShape*, VectorComparator> BoxMapTy;
typedef std::map<btScalar, CollisionShape*> SphereMapTy;
typedef std::set<DynamicsWorld*> WorldSetTy;
typedef BoxMapTy::iterator BoxMapTyIt;
typedef SphereMapTy::iterator SphereMapTyIt;
typedef WorldSetTy::iterator WorldSetTyIt;

  btDefaultCollisionConfiguration* CollisionC;
  btCollisionDispatcher* CDispatcher;
  btBroadphaseInterface* Broadphase;
  btSequentialImpulseConstraintSolver* ConstraintSolver;
  BoxMapTy BoxMap;
  SphereMapTy SphereMap;
  WorldSetTy WorldSet;

public:
  PhysicsManager();
  ~PhysicsManager();
 
  CollisionShape *GetBoxShape(float x_size, float y_size, float z_size); 
  CollisionShape *GetSphereShape(float radius);
  DynamicsWorld *GetNewDynamicsWorld();
  void DisposeDynamicsWorld(DynamicsWorld *world);
};


}

#endif
