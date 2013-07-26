#ifndef __UBERNGINE_EAGLSYSTEMFACILITIES_H__
#define __UBERNGINE_EAGLSYSTEMFACILITIES_H__

#import <QuartzCore/CAEAGLLayer.h>
#import <QuartzCore/CADisplayLink.h>
#include <OGLSystemFacilities.h>

namespace Uberngine {

class EAGLSystemFacilities : public OGLSystemFacilities<EAGLSystemFacilities> {

  CAEAGLLayer* EAGLLayer;
  CADisplayLink* DisplayLink;
  EAGLContext* GLCtx;
public:
  EAGLSystemFacilities(CAEAGLLayer *layer, CADisplayLink* display_link);
  bool CreateAndSetRenderContext(int *width, int *height, int c_bits, int d_bits, int s_bits, 
                                         bool fullscreen);
  CFTimeInterval GetTime();
  void SwapBuffers();
  void SetWindowTitle(const char *title);
  ~EAGLSystemFacilities();

};

}

#endif
