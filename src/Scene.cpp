#include <Node.h>
#include <Camera.h>
#include <Light.h>
#include <Scene.h>
#include <Engine.h>
#include <Physics/DynamicsWorld.h>

using namespace Uberngine;

Scene::Scene(BaseEngine *e) : Node(e, NULL), Cam(NULL) {
  DynWorld = e->GetPhysicsManager()->GetNewDynamicsWorld();
}

Scene::~Scene() {
  //Delete lights
  for (LightListIt I = Lights.begin(), E = Lights.end(); I != E; ++I) 
    delete *I;

  //Delete camera
//  if (Cam)
//    delete Cam;
}

void Scene::RenderScene() {
  Render(this);
}

void Scene::UpdateScene() {
  GlobalUpdate();
  DynWorld->StepWorld(Eng.GetFrameTimeDelta(), 10);
}

void Scene::SetGravity(float x, float y, float z) { DynWorld->SetGravity(x,y,z); }
