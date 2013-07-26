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
  inline GLuint GetDrawFramebufferObject() const { return DrawFramebuffer; }
  inline void SetDrawFramebufferObject(GLuint obj) { DrawFramebuffer = obj; }
  inline GLuint GetColorRenderbuffer() const { return Renderbuffers[0]; }
  inline GLuint GetDepthRenderbuffer() const { return Renderbuffers[1]; }
};

}
#endif
