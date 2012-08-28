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
public:
  Engine<UBE_IOS>(UIView *layer) : IosView(layer) {
    Sys = static_cast<OGLSystemFacilities*>(SystemFacilitiesFactory<UBE_IOS>::Create((CAEAGLLayer*) IosView.layer)); 
  }
  bool Init(int width, int height, int c_bits, int d_bits, int s_bits) {
    bool ret_val = false;
    ret_val = Sys->CreateAndSetRenderContext(&width, &height, c_bits, d_bits, s_bits, false);
    if (ret_val) {
      Width = width;
      Height = height;
      Depth = c_bits;
      if (d_bits != 0) {
        glEnable(GL_DEPTH_TEST);
        DepthTestEnabled = true;
      }
    }
    return ret_val;
  }
  GLuint GetFramebuffer() { return Sys->GetDrawFramebufferObject(); }
  GLuint GetColorRenderbuffer() { return Sys->GetColorRenderbuffer(); }
  EAGLSystemFacilities *GetSysFacilities() { return static_cast<EAGLSystemFacilities*>(Sys); }
};

}
#endif
