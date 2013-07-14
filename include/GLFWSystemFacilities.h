#ifndef __UBERNGINE_GLFWSYSTEMFACILITIES_H__
#define __UBERNGINE_GLFWSYSTEMFACILITIES_H__

#include <OGLSystemFacilities.h>

namespace {
  class GLFWwindow;
}
namespace Uberngine {

class GLFWSystemFacilities : public OGLSystemFacilities {

  GLFWwindow *window;

public:
  GLFWSystemFacilities();
  virtual bool CreateAndSetRenderContext(int *width, int *height, int c_bits, int d_bits, int s_bits, 
                                         bool fullscreen);
  virtual void GetMousePosition(int *x, int *y);
  virtual bool GetPressedKey(int key);
  virtual float GetTime();
  virtual void SwapBuffers();
  virtual void SetWindowTitle(const char *title);
  virtual ~GLFWSystemFacilities();

};

}

#endif
