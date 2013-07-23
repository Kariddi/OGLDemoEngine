#include <GLFWEngine.h>

using namespace Uberngine;

Engine<UBE_GLFW>::Engine() : Gui(nullptr) {}

bool Engine<UBE_GLFW>::Init(const char *window_title, int width, int height, int c_bits, 
                      int d_bits, int s_bits, bool fullscreen) {
  bool ret_val = this->Sys.CreateAndSetRenderContext(&width, &height, c_bits, d_bits, s_bits, fullscreen);

  if (ret_val) {
    this->Sys.SetWindowTitle(window_title);
    this->Width = width;
    this->Height = height;
    this->Depth = c_bits;
#ifndef MAC_SYS
    glewInit();
#endif
    if (d_bits != 0) {
      glEnable(GL_DEPTH_TEST);
      this->DepthTestEnabled = true;
    }
  }
  return ret_val;  

}

void Engine<UBE_GLFW>::SetPressedKeyCallback(KeyCallbackTy kc) {
  this->Sys.SetPressedKeyCallback(kc);
}

//Renders the loaded scene
void Engine<UBE_GLFW>::RenderScene() {
  glBindFramebuffer(GL_FRAMEBUFFER, Sys.GetDrawFramebufferObject());
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  if (DepthTestEnabled) {
    glClearDepth(1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);
  }
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
