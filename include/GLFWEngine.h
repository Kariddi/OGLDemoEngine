#ifndef __UBERNGINE_ENGINE_UBE_GLFW_H__
#define __UBERNGINE_ENGINE_UBE_GLFW_H__
#include <GL/internal.h>
#include <GLFWSystemFacilities.h>
#include <Engine.h>
#include <Scene.h>

namespace Uberngine {

template<>
class Engine<UBE_GLFW> : public BaseEngine<Engine<UBE_GLFW>> {
  GUI* Gui;
  GLFWSystemFacilities Sys;
public:
  Engine();
  bool Init(const char *window_title, int width, int height, int c_bits, 
                      int d_bits, int s_bits, bool fullscreen);
  bool CheckPressedKey(int key) { return Sys.GetPressedKey(key); }
  //Retrieves the mouse position
  void GetMousePosition(int* x, int* y) { Sys.GetMousePosition(x, y); }
  //Renders the loaded scene
  void RenderScene();

  void StepSingleFrame();

  //Implements the engine Main loop
  void MainLoop();

  GUI* GetGUI() {
    if (Gui == nullptr)
      Gui = new GUI;
    return Gui;
  }

  void RenderGUI() { Gui->RenderGUI(); }

};

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

}

#endif
