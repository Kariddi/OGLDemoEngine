#ifndef __UBERNGINE_GLFWSYSTEMFACILITIES_H__
#define __UBERNGINE_GLFWSYSTEMFACILITIES_H__

#include <OGLSystemFacilities.h>

namespace Uberngine {

class GLFWSystemFacilities : public OGLSystemFacilities {
public:
  GLFWSystemFacilities();
  virtual bool CreateAndSetRenderContext(int width, int height, int r_bits, int g_bits, int b_bits, int a_bits, int d_bits, int s_bits, bool mode);
  virtual void GetMousePosition(int *x, int *y);
  virtual bool GetPressedKey(int key);
  virtual float GetTime();
  virtual void SwapBuffers();
  virtual void SetWindowTitle(const char *title);
  virtual ~GLFWSystemFacilities();

};

}

#endif
