#ifndef __UBERNGINE_OGLSYSTEMFACILITIES_H__
#define __UBERNGINE_OGLSYSTEMFACILITIES_H__

#include <OGL.h>
#include <SystemFacilities.h>

namespace Uberngine {

template<typename Derived>
class OGLSystemFacilities : public SystemFacilities<Derived> {
protected:
  GLuint DrawFramebuffer;
  GLuint Renderbuffers[2];
public:
  OGLSystemFacilities(GLuint dfb) : SystemFacilities<Derived>(), DrawFramebuffer(0) { 
    Renderbuffers[0] = 0;  
    Renderbuffers[1] = 0;
  }
  inline GLuint GetDrawFramebufferObject() { return DrawFramebuffer; }
  inline GLuint GetColorRenderbuffer() { return Renderbuffers[0]; } 
};

}
#endif
