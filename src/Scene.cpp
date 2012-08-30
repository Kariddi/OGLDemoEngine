#include <Node.h>
#include <Camera.h>
#include <Light.h>
#include <Scene.h>

using namespace Uberngine;

Scene::Scene(BaseEngine *e) : Node(e, NULL), Cam(NULL) {}

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
}
