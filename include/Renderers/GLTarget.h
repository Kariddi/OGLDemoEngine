#ifndef __UBERNGINE_GLTARGET_H__
#define __UBERNGINE_GLTARGET_H__

#include <Renderers/RenderingTarget.h>
#include <Defs.h>
#include <OGL.h>
#include <cstdint>

struct TextureData;

namespace Uberngine {

class GLRenderingTarget : public PureRenderingTarget {

public:
  enum class TargetType : std::uint8_t {
    Texture, Buffer, Default
  };

protected:
  GLuint GLObject;
  TargetType Type;

  GLRenderingTarget(std::uint16_t width, std::uint16_t height, TargetType tt) : PureRenderingTarget(width, height), GLObject(0), Type(tt) {}
  GLRenderingTarget(std::uint16_t width, std::uint16_t height, TargetType tt, GLuint gl_object) : PureRenderingTarget(width, height), GLObject(gl_object), Type(tt) {}

public:
  
  GLuint GetGLObject() const { return GLObject; }
  TargetType GetType() const { return Type; }

};

}

#endif
