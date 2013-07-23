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
  GUI* Gui;
  EAGLSystemFacilities Sys;
public:
  Engine<UBE_IOS>(UIView *layer) : IosView(layer), Gui(nullptr), Sys(IosView.layer) {}
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

  GUI* GetGUI() {
    if (Gui == nullptr)
      Gui = new GUI;
    return Gui;
  }

  void RenderGUI() { Gui->RenderGUI(); }
};

}
#endif
