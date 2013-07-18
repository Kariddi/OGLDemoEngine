#ifndef __UBERNGINE_EAGLSYSTEMFACILITIES_H__
#define __UBERNGINE_EAGLSYSTEMFACILITIES_H__

#import <QuartzCore/CAEAGLLayer.h>
#include <OGLSystemFacilities.h>

namespace Uberngine {

class EAGLSystemFacilities : public OGLSystemFacilities<EAGLSystemFacilities> {

  CAEAGLLayer *EAGLLayer;
  EAGLContext *GLCtx;
public:
  EAGLSystemFacilities(CAEAGLLayer *layer);
  bool CreateAndSetRenderContext(int *width, int *height, int c_bits, int d_bits, int s_bits, 
                                         bool fullscreen);
  float GetTime();
  void SwapBuffers();
  void SetWindowTitle(const char *title);
  ~EAGLSystemFacilities();

};

}

#endif
