#ifndef __UBERNGINE_IOSENGINE_H__
#define __UBERNGINE_IOSENGINE_H__
#include <IOSSystemFacilitiesFactory.h>
#include <Engine.h>
#import <UIKit/UIView.h>
#import <QuartzCore/CAEAGLLayer.h>

namespace Uberngine {

template<>
class Engine<UBE_IOS> : public BaseEngine {
  UIView *IosView;
  EAGLSystemFacilities Sys;
public:
  Engine<UBE_IOS>(UIView *layer) : IosView(layer), Sys(IosView.layer) {}
  bool Init(int width, int height, int c_bits, int d_bits, int s_bits) {
    bool ret_val = false;
    ret_val = this->Sys->CreateAndSetRenderContext(&width, &height, c_bits, d_bits, s_bits, false);
    if (ret_val) {
      this->Width = width;
      this->Height = height;
      this->Depth = c_bits;
      if (d_bits != 0) {
        glEnable(GL_DEPTH_TEST);
        DepthTestEnabled = true;
      }
    }
    return ret_val;
  }
  GLuint GetFramebuffer() { return this->Sys.GetDrawFramebufferObject(); }
  GLuint GetColorRenderbuffer() { return this->Sys.GetColorRenderbuffer(); }
  EAGLSystemFacilities *GetSysFacilities() { return static_cast<EAGLSystemFacilities*>(&this->Sys); }
    //Renders the loaded scene
  void RenderScene();

  void StepSingleFrame();

  //Implements the engine Main loop
  void MainLoop();
};

//Renders the loaded scene
void Engine<UBE_IOS>::RenderScene() {
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

}
#endif
