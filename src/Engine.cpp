#include <Node.h>
#include <Scene.h>
#include <Engine.h>

using namespace std;
using namespace Uberngine;

BaseEngine::BaseEngine() : CurrScene(0), DepthTestEnabled(false) {
  //glfwInit();
  //if (Sys == NULL)
  //  Sys = SystemFacilitiesFactory::Create(UBE_OGL); 
}

/*#ifdef IOS_SYS
Engine::Engine(UIView *view) {
  Engine();
  Sys = SystemFacilitiesFactory::Create(UBE_OGL, view);
}
#endif
*/

BaseEngine::~BaseEngine() {
  for (SceneListIt I = Scenes.begin(), E = Scenes.end(); I != E; ++I)
    delete *I;

  BaseSystemFacilitiesFactory::Destroy();
}

//Initializes the Engine
//Creates a new Scene
Scene *BaseEngine::CreateNewScene() {

  Scene *NewScene = new Scene(this);
  Scenes.push_back(NewScene);

  return NewScene;
}

//Updates the currently loaded scene
void BaseEngine::UpdateScene() {
  Scenes[CurrScene]->UpdateScene();
}

//Renders the loaded scene
void BaseEngine::RenderScene() {
  glBindFramebuffer(GL_FRAMEBUFFER, Sys->GetDrawFramebufferObject());
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  if (DepthTestEnabled) {
    glClearDepth(1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);
  }
  Scenes[CurrScene]->RenderScene();
  //glfwSwapBuffers();
  Sys->SwapBuffers();
}

void BaseEngine::StepSingleFrame() {
  PreviousFrameTime = FrameTime;
  FrameTime = Sys->GetTime();
  FrameTimeDelta = FrameTime - PreviousFrameTime;
  this->UpdateScene();
  this->RenderScene();    
 
}

//Implements the engine Main loop
void BaseEngine::MainLoop() {
  while(1) {
    StepSingleFrame();
  }

}
