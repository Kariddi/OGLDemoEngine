#include <Node.h>
#include <assert.h>
#include <Scene.h>
#include <Engine.h>
#include <Physics/DynamicsWorld.h>
#include <algorithm>

using namespace Uberngine;

Scene::Scene(PhysicsManager* p_manager) 
  : RootNode(new Node), Cam(nullptr), PhysMan(p_manager) {

  DynWorld = p_manager->GetNewDynamicsWorld();
  //RootNode->SetDynamicsWorld(DynWorld);
}

Scene::~Scene() {
  //Delete lights
  for (auto I : Lights) 
    delete I;

  for (auto I : Nodes) { 
    delete I;
  }
  Nodes.clear();

  delete RootNode;

  PhysMan->DisposeDynamicsWorld(DynWorld);
  DynWorld = nullptr;
}

void Scene::RenderScene() {
  for (auto Node : Nodes) {
    Node->GetRenderer().Render(Cam->GetViewMatrix(), Cam->GetProjMatrix(), 
      Node->GetComulative(), Lights);
  }
}

void Scene::UpdateScene(float frame_time_delta) {
  //GlobalUpdate();
  RootNode->GlobalUpdate();
  DynWorld->StepWorld(frame_time_delta, 10);
}

void Scene::SetGravity(float x, float y, float z) { DynWorld->SetGravity(x,y,z); }

void Scene::SetRigidBodyForNode(RigidBody* rb, Node* n) {
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

void Scene::AttachNodeToParent(Node& child, Node& parent) {
  Scene* SC = child.GetScene();
  if (SC == nullptr || SC != this) {
    if (SC != nullptr)
      SC->DetachNode(child);
    child.SetScene(this);
    Nodes.push_back(&child);
  }
  parent.AddChildNode(&child);
  SetRigidBodyForNode(child.GetRigidBody(), &child);
}

void Scene::DetachNode(const Node& child) {
  Node::NodeListIt It = std::find(Nodes.begin(), Nodes.end(), &child);
  assert(It != Nodes.end() && "Node not present during detach");
  Nodes.erase(It);
}

void Scene::AttachNodeToScene(Node& child) {
  Scene* SC = child.GetScene();
  if (SC != nullptr)
    SC->DetachNode(child);
  AttachNodeToParent(child, *RootNode);
}
