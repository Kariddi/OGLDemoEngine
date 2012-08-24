#ifndef __UBERNGINE_SYSTEMFACILITIES_H__
#define __UBERNGINE_SYSTEMFACILITIES_H__

namespace Uberngine {

class SystemFacilities {
public:
  virtual bool CreateAndSetRenderContext(int width, int height, int r_bits, int g_bits, int b_bits, int a_bits, int d_bits, int s_bits, bool mode) = 0;
  virtual void GetMousePosition(int *x, int *y) = 0;
  virtual bool GetPressedKey(int key) = 0;
  virtual float GetTime() = 0;
  virtual void SwapBuffers() = 0;
  virtual void SetWindowTitle(const char *title) = 0;
  virtual ~SystemFacilities() {};
};

}

#endif
