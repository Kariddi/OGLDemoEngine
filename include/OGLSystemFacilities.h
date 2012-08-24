#ifndef __UBERNGINE_OGLSYSTEMFACILITIES_H__
#define __UBERNGINE_OGLSYSTEMFACILITIES_H__

#include <OGL.h>
#include <SystemFacilities.h>

namespace Uberngine {

class OGLSystemFacilities : public SystemFacilities {
protected:
  GLuint DrawFramebuffer;
public:
  OGLSystemFacilities(GLuint dfb) : SystemFacilities(), DrawFramebuffer(0) {}
  inline GLuint GetDrawFramebufferObject() { return DrawFramebuffer; } 
};

}
#endif
