#ifndef __UBERNGINE_GLBUFFERTARGET_H__
#define __UBERNGINE_GLBUFFERTARGET_H__

#include <Renderers/RenderingTarget.h>
#include <Renderers/GLTarget.h>
#include <Defs.h>
#include <OGL.h>
#include <cstdint>

namespace Uberngine {

template<>
class RenderingTarget<RendererTypes::OpenGL, RenderTargetType::Buffer> : public GLRenderingTarget {

public:
  RenderingTarget(std::uint16_t width, std::uint16_t height, ColorType ct);
  RenderingTarget(std::uint16_t width, std::uint16_t height, GLuint gl_object);
  ~RenderingTarget(); 

};

}

#endif
