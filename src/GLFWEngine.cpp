#include <GLFWEngine.h>
#include <Renderers/GLDefaultTarget.h>

using namespace Uberngine;

Engine<UBE_GLFW>::Engine() : Gui(nullptr), DefaultSurface(nullptr) {}

Engine<UBE_GLFW>::~Engine() {

  if (DefaultSurface != nullptr)
    delete DefaultSurface;

  for (auto S : DefaultRenderingTargets)
    delete S;
  for (auto S : TextureRenderingTargets)
    delete S;
  for (auto S : BufferRenderingTargets)
    delete S;

}

bool Engine<UBE_GLFW>::Init(const char *window_title, int width, int height, int c_bits, 
                      int d_bits, int s_bits, bool fullscreen) {
  bool ret_val = this->Sys.CreateAndSetRenderContext(&width, &height, c_bits, d_bits, s_bits, fullscreen);

  const DefaultRenderingTargetTy* ColorTarget = new DefaultRenderingTargetTy(width, height);
  const DefaultRenderingTargetTy* DepthTarget = nullptr;

  DefaultRenderingTargets.push_back(ColorTarget);
  if (ret_val) {
    this->Sys.SetWindowTitle(window_title);
    this->Width = width;
    this->Height = height;
    this->Depth = c_bits;
#ifndef MAC_SYS
    glewInit();
#endif
    if (d_bits != 0) {
      DepthTarget = new DefaultRenderingTargetTy(width, height);
      DefaultRenderingTargets.push_back(DepthTarget);
      this->DepthTestEnabled = true;
    }
  }

  DefaultSurface = new RenderingSurface<RendererTypes::OpenGL>(ColorTarget, DepthTarget);

  return ret_val;  

}

Scene<RendererTypes::OpenGL>* Engine<UBE_GLFW>::CreateNewScene() {
  Scene<RendererTypes::OpenGL>* NewScene = 
    new Scene<RendererTypes::OpenGL>(&PhysMan, DefaultSurface);

  Scenes.push_back(NewScene);
  return NewScene;
}

void Engine<UBE_GLFW>::SetPressedKeyCallback(KeyCallbackTy kc) {
  this->Sys.SetPressedKeyCallback(kc);
}

//Renders the loaded scene
void Engine<UBE_GLFW>::RenderScene() {
//  glBindFramebuffer(GL_FRAMEBUFFER, Sys.GetDrawFramebufferObject());
  Scenes[CurrScene]->RenderScene();
  if (Gui != NULL)
    RenderGUI();
  //glfwSwapBuffers();
  Sys.SwapBuffers();
}

void Engine<UBE_GLFW>::StepSingleFrame() {
  PreviousFrameTime = FrameTime;
  FrameTime = Sys.GetTime();
  FrameTimeDelta = FrameTime - PreviousFrameTime;
  this->UpdateScene();
  this->RenderScene();
}

//Implements the engine Main loop
void Engine<UBE_GLFW>::MainLoop() {

  while(1) {
    StepSingleFrame();
  }

}
