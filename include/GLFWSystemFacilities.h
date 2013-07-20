#ifndef __UBERNGINE_GLFWSYSTEMFACILITIES_H__
#define __UBERNGINE_GLFWSYSTEMFACILITIES_H__

#include <OGLSystemFacilities.h>
#include <functional>

/*namespace {
  class GLFWwindow;
}*/
namespace Uberngine {

class GLFWSystemFacilities : public OGLSystemFacilities<GLFWSystemFacilities> {

  GLFWwindow *window;

public:
  typedef void(*KeyCallbackTy)(GLFWwindow*, int, int, int, int);
  GLFWSystemFacilities();
  bool CreateAndSetRenderContext(int *width, int *height, int c_bits, int d_bits, int s_bits, 
                                         bool fullscreen);
  void GetMousePosition(int *x, int *y);
  void SetPressedKeyCallback(KeyCallbackTy kc);
  bool GetPressedKey(int key);
  float GetTime();
  void SwapBuffers();
  void SetWindowTitle(const char *title);
  ~GLFWSystemFacilities();

};

}

#endif
