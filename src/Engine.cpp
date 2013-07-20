#include <Node.h>
#include <Scene.h>
#include <GUI/GUI.h>
#include <Engine.h>

using namespace std;
using namespace Uberngine;

PureEngine::PureEngine() : CurrScene(0), DepthTestEnabled(false),
                           FrameTime(0.0f),
                           PreviousFrameTime(0.0f), 
                           FrameTimeDelta(0.0f),
                           Width(0), Height(0), Depth(0) {
  //PhysMan = new PhysicsManager;
}

PureEngine::~PureEngine() {
  //delete PhysMan;

  //BaseSystemFacilitiesFactory::Destroy();
}

//Initializes the Engine

