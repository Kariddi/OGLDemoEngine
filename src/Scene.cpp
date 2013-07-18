#include <Node.h>
#include <Camera.h>
#include <Light.h>
#include <Scene.h>
#include <Engine.h>
#include <Physics/DynamicsWorld.h>

using namespace Uberngine;

Scene::Scene(PhysicsManager* p_manager) 
  : RootNode(new Node(this)), Cam(nullptr), PhysMan(p_manager) {

  DynWorld = p_manager->GetNewDynamicsWorld();
  //RootNode->SetDynamicsWorld(DynWorld);
}

Scene::~Scene() {
  //Delete lights
  for (auto I : Lights) 
    delete I;

  delete RootNode;

  PhysMan->DisposeDynamicsWorld(DynWorld);
  DynWorld = nullptr;
}

void Scene::RenderScene() {
  RootNode->Render(this);
}

void Scene::UpdateScene(float frame_time_delta) {
  //GlobalUpdate();
  RootNode->GlobalUpdate();
  DynWorld->StepWorld(frame_time_delta, 10);
}

void Scene::SetGravity(float x, float y, float z) { DynWorld->SetGravity(x,y,z); }
