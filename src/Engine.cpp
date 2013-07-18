#include <Node.h>
#include <Scene.h>
#include <GUI/GUI.h>
#include <Engine.h>

using namespace std;
using namespace Uberngine;

PureEngine::PureEngine() : Gui(NULL), CurrScene(0), DepthTestEnabled(false),
                           FrameTime(0.0f),
                           PreviousFrameTime(0.0f), 
                           FrameTimeDelta(0.0f),
                           Width(0), Height(0), Depth(0) {
  PhysMan = new PhysicsManager;
}

PureEngine::~PureEngine() {
  for (auto I : Scenes) {
    delete I;
  }

  delete PhysMan;

  //BaseSystemFacilitiesFactory::Destroy();
}

GUI* PureEngine::GetGUI() {
  if (Gui == NULL)
    Gui = new GUI;
  return Gui;
}

//Initializes the Engine
//Creates a new Scene
Scene* PureEngine::CreateNewScene() {

  Scene* NewScene = new Scene(PhysMan);
  Scenes.push_back(NewScene);

  return NewScene;
}

//Updates the currently loaded scene
void PureEngine::UpdateScene() {
  Scenes[CurrScene]->UpdateScene(GetFrameTimeDelta());
}