#ifndef __UBERNGINE_PHYSICSMANAGER_H__
#define __UBERNGINE_PHYSICSMANAGER_H__

#include <map>
#include <LinearMath/btVector3.h>

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btBoxShape;
class btSphereShape;
class btCollisionShape;

namespace Uberngine {

class PhysicsManager {
typedef std::map<btVector3, btBoxShape*> BoxMapTy;
typedef std::map<btScalar, btSphereShape*> SphereMapTy;
typedef BoxMapTy::iterator BoxMapTyIt;
typedef SphereMapTy::iterator SphereMapTyIt;

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

  btDefaultCollisionConfiguration* CollisionC;
  btCollisionDispatcher* CDispatcher;
  btBroadphaseInterface* Broadphase;
  btSequentialImpulseConstraintSolver* ConstraintSolver;
  BoxMapTy BoxMap;
  SphereMapTy SphereMap;

public:
  PhysicsManager();
  ~PhysicsManager();
 
  btCollisionShape *GetBoxShape(float x_size, float y_size, float z_size); 
  btCollisionShape *GetSphereShape(float radius);
};

}

#endif
