#ifndef __UBERNGINE_ENGINE_UBE_GLFW_H__
#define __UBERNGINE_ENGINE_UBE_GLFW_H__
#include <GLFWSystemFacilitiesFactory.h>
#include <Engine.h>

namespace Uberngine {

template <>
class Engine<UBE_GLFW> : public BaseEngine {
public:
  Engine();
  bool Init(const char *window_title, int width, int height, int c_bits, 
                      int d_bits, int s_bits, bool fullscreen);
};

Engine<UBE_GLFW>::Engine() {
  if (Sys == NULL)
    Sys = SystemFacilitiesFactory<UBE_GLFW>::Create(); 
}

bool Engine<UBE_GLFW>::Init(const char *window_title, int width, int height, int c_bits, 
                      int d_bits, int s_bits, bool fullscreen) {
  int r_bits, g_bits, b_bits, a_bits;
  switch (c_bits) {
    case 32:
      r_bits = 8;
      g_bits = 8;
      b_bits = 8;
      a_bits = 8;
      break;
    case 24:
      r_bits = 8;
      g_bits = 8;
      b_bits = 8;
      a_bits = 0;
      break;
    case 16:
      r_bits = 5;
      g_bits = 6;
      b_bits = 5;
      a_bits = 0;
      break;
    default:
      return false;
  }
  int ret_val = Sys->CreateAndSetRenderContext(width, height, r_bits, g_bits, b_bits, a_bits, d_bits, s_bits, fullscreen);

  if (ret_val == GL_TRUE) {
    Sys->SetWindowTitle(window_title);
#ifndef MAC_SYS
    glewInit();
#endif
    if (d_bits != 0) {
      glEnable(GL_DEPTH_TEST);
      DepthTestEnabled = true;
    }

  }
  return ret_val == GL_TRUE ? true : false;  

}

}

#endif
