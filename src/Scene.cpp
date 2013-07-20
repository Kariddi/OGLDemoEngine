#include <Node.h>
#include <assert.h>
#include <Scene.h>
#include <Engine.h>

using namespace Uberngine;

PureScene::PureScene(PhysicsManager* p_manager) 
  : PhysMan(p_manager) {

  DynWorld = p_manager->GetNewDynamicsWorld();
  //RootNode->SetDynamicsWorld(DynWorld);
}

PureScene::~PureScene() {
  //Delete lights
  for (auto I : Lights) 
    delete I;

  PhysMan->DisposeDynamicsWorld(DynWorld);
  DynWorld = nullptr;
}

void PureScene::SetGravity(float x, float y, float z) { DynWorld->SetGravity(x,y,z); }

void PureScene::SetRigidBodyForNode(RigidBody* rb, PureNode* n) {
  DynamicsWorld& DWorld = *GetDynamicsWorld();
  RigidBody* NodeRB = n->GetRigidBody();
  if (NodeRB != nullptr) {
    DWorld.RemoveRigidBody(NodeRB);
  }
  if (rb != nullptr) {
    rb->SetKinematic(true);
    rb->SetPosition(n->GetTransform());
    rb->SetKinematic(false);
    DWorld.AddRigidBody(rb);
  }
  n->SetRigidBody(rb);
}
