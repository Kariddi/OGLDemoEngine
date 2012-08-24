#ifndef __UBERNGINE_EAGLSYSTEMFACILITIES_H__
#define __UBERNGINE_EAGLSYSTEMFACILITIES_H__

#import <QuartzCore/CAEAGLLayer.h>
#include <OGLSystemFacilities.h>

namespace Uberngine {

class EAGLSystemFacilities : public OGLSystemFacilities {

  CAEAGLLayer *EAGLLayer;
  EAGLContext *GLCtx;
public:
  EAGLSystemFacilities(CAEAGLLayer *layer);
  virtual bool CreateAndSetRenderContext(int *width, int *height, int c_bits, int d_bits, int s_bits, 
                                         bool fullscreen);
  virtual void GetMousePosition(int *x, int *y);
  virtual bool GetPressedKey(int key);
  virtual float GetTime();
  virtual void SwapBuffers();
  virtual void SetWindowTitle(const char *title);
  virtual ~EAGLSystemFacilities();


};

}

#endif
