#ifndef __UBERNGINE_ENGINE_UBE_GLFW_H__
#define __UBERNGINE_ENGINE_UBE_GLFW_H__
#include <GLFWSystemFacilitiesFactory.h>
#include <Engine.h>

namespace Uberngine {

template<>
class Engine<UBE_GLFW> : public BaseEngine {
public:
  Engine();
  bool Init(const char *window_title, int width, int height, int c_bits, 
                      int d_bits, int s_bits, bool fullscreen);
};

Engine<UBE_GLFW>::Engine() {
  this->Sys = static_cast<OGLSystemFacilities*>(SystemFacilitiesFactory<UBE_GLFW>::Create()); 
}

bool Engine<UBE_GLFW>::Init(const char *window_title, int width, int height, int c_bits, 
                      int d_bits, int s_bits, bool fullscreen) {
  bool ret_val = this->Sys->CreateAndSetRenderContext(&width, &height, c_bits, d_bits, s_bits, fullscreen);

  if (ret_val) {
    this->Sys->SetWindowTitle(window_title);
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

}

#endif
