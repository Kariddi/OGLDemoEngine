#include <IOSEngine.h>

using namespace Uberngine;

Scene<RendererTypes::OpenGL>* Engine<UBE_IOS>::CreateNewScene() {
    Scene<RendererTypes::OpenGL>* NewScene =
    new Scene<RendererTypes::OpenGL>(&PhysMan, DefaultSurface);
    
    Scenes.push_back(NewScene);
    return NewScene;
}

//Renders the loaded scene
void Engine<UBE_IOS>::RenderScene() {
  Scenes[CurrScene]->RenderScene();
  if (Gui != NULL)
    RenderGUI();
  //glfwSwapBuffers();
  Sys.SwapBuffers();
}

void Engine<UBE_IOS>::StepSingleFrame() {
  PreviousFrameTime = FrameTime;
  FrameTime = Sys.GetTime();
  FrameTimeDelta = FrameTime - PreviousFrameTime;
  this->UpdateScene();
  this->RenderScene();
}

//Implements the engine Main loop
void Engine<UBE_IOS>::MainLoop() {

  while(1) {
    StepSingleFrame();
  }

}
