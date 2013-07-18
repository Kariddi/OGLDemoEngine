#ifndef __UBERNGINE_SYSTEMFACILITIES_H__
#define __UBERNGINE_SYSTEMFACILITIES_H__

namespace Uberngine {

template<typename Derived>
class SystemFacilities {
public:
  bool CreateAndSetRenderContext(int *width, int *height, int c_bits, int d_bits, int s_bits, bool fullscreen) {
    static_cast<Derived*>(this)->CreateAndSetRenderContext(width, height, c_bits, d_bits, s_bits, fullscreen);
  }
  float GetTime() {
  	static_cast<Derived*>(this)->GetTime();
  }
  void SwapBuffers() {
  	static_cast<Derived*>(this)->SwapBuffers();
  }
  void SetWindowTitle(const char *title) {
  	static_cast<Derived*>(this)->SetWindowTitle(title);
  }
  // No virtual destructor because this class shouldn't be used directly (as a pointer)
};

}

#endif
