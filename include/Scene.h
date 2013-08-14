#ifndef __UBERNGINE_SCENE_H__
#define __UBERNGINE_SCENE_H__
#include <vector>
#include <Node.h>
#include <Light.h>
#include <Camera.h>
#include <Renderers/GLRenderPass.h>
#include <Physics/DynamicsWorld.h>
#include <algorithm>

namespace Uberngine {

template<typename RendererType>
class Camera;
class PhysicsManager;
class RigidBody;

class PureScene {
  PhysicsManager* PhysMan;

protected:
  LightList Lights;
  DynamicsWorld* DynWorld;
  PureScene(PhysicsManager* p_manager);
  ~PureScene();
public:
  void AddLight(Light* lht) { Lights.push_back(lht); }
  //Gets the list of all the lights
  const LightList& GetLights() const { return Lights; }
  void SetGravity(float x, float y, float z);
  void SetRigidBodyForNode(RigidBody* rb, PureNode* n);
  DynamicsWorld* GetDynamicsWorld() { return DynWorld; }
};

template<typename RendererType>
class Scene : public PureScene {
public:
  typedef RenderPass<RendererType> RenderPassTy;
  typedef std::vector<RenderPassTy> RenderPassList;
private:
  typename Node<RendererType>::NodeList Nodes;
  Node<RendererType>* RootNode;
  Camera<RendererType>* Cam;
  RenderPassList RenderPasses;

  Scene(PhysicsManager* p_manager);
  void DetachNode(const Node<RendererType>& child);
  template<typename Derived>
  friend class BaseEngine;
public:

  ~Scene();
  //Starts the rendering process of the scene
  void RenderScene();
  //Starts the update of the scene
  void UpdateScene(float frame_time_delta);
  //Sets the camera of the scene
  void SetCamera(Camera<RendererType>* cam) { Cam = cam; RootNode->AddChildNode(cam); }
  //Gets the camera of the scene
  Camera<RendererType>* GetCamera() const { return Cam; }
  //Adds a light to the scene
  void AttachNodeToParent(Node<RendererType>& child, Node<RendererType>& parent);
  void AttachNodeToScene(Node<RendererType>& child);
  Node<RendererType>* CreateNewNode();
  Node<RendererType>* GetRootNode() const { return RootNode; }
  //void AssociateShader(MeshId mid, Shader sh);
};

template<typename RendererType>
Scene<RendererType>::Scene(PhysicsManager* p_manager) 
  : PureScene(p_manager), RootNode(new Node<RendererType>), Cam(nullptr) {}

template<typename RendererType>
Scene<RendererType>::~Scene() {
  for (auto I : Nodes) { 
    delete I;
  }
  Nodes.clear();

  delete RootNode;
}

template<typename RendererType>
void Scene<RendererType>::RenderScene() {

  //for (auto RenderPass : RenderPasses) {
  //  RenderPass.Render(Cam->GetViewMatrix(), Cam->GetProjMatrix(),
  //    Lights);
  //}

  for (auto Node : Nodes) {
    Node->GetRenderer().Render(Cam->GetViewMatrix(), Cam->GetProjMatrix(), 
      Lights, 0);
  }
}

template<typename RendererType>
void Scene<RendererType>::UpdateScene(float frame_time_delta) {
  //GlobalUpdate();
  RootNode->GlobalUpdate();
  DynWorld->StepWorld(frame_time_delta, 10);
}


template<typename RendererType>
void Scene<RendererType>::AttachNodeToParent(Node<RendererType>& child, Node<RendererType>& parent) {
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

template<typename RendererType>
void Scene<RendererType>::DetachNode(const Node<RendererType>& child) {
  typename Node<RendererType>::NodeListIt It = std::find(Nodes.begin(), Nodes.end(), &child);
  assert(It != Nodes.end() && "Node not present during detach");
  Nodes.erase(It);
}

template<typename RendererType>
void Scene<RendererType>::AttachNodeToScene(Node<RendererType>& child) {
  Scene* SC = child.GetScene();
  if (SC != nullptr)
    SC->DetachNode(child);
  AttachNodeToParent(child, *RootNode);
}

template<typename RendererType>
Node<RendererType>* Scene<RendererType>::CreateNewNode() {
  Node<RendererType>* NewNode = new Node<RendererType>();
  AttachNodeToScene(*NewNode);

  return NewNode;
}

}
#endif
