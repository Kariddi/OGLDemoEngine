#ifndef __UBERNGINE_GLTEXTURETARGET_H__
#define __UBERNGINE_GLTEXTURETARGET_H__

#include <Renderers/RenderingTarget.h>
#include <Defs.h>
#include <OGL.h>
#include <cstdint>

struct TextureData;

namespace Uberngine {

template<>
class RenderingTarget<RendererTypes::OpenGL, RenderTargetType::Texture> : public PureRenderingTarget {

  GLuint Texture;
public:
  RenderingTarget(std::uint16_t width, std::uint16_t height, ColorType ct, void* td);
  RenderingTarget(std::uint16_t width, std::uint16_t height, ColorType ct);
  ~RenderingTarget(); 

  GLuint GetGLObject() const { return Texture; }

};

}

#endif
