#ifndef __UBERNGINE_RENDERINGTARGET_H__
#define __UBERNGINE_RENDERINGTARGET_H__

#include <cstdint>

namespace Uberngine {

namespace RenderTargetType {

  struct Texture;
  struct Buffer;
  struct Default;

}

// Lets match OpenGL supported types for now ...

enum class ColorType : std::uint8_t {

  INVALID,
  RGB,
  RGBA,
  GRAY,
  GRAY_ALPHA,
  ALPHA,
  DEPTH

};

class PureRenderingTarget {

protected:
  std::uint16_t Width;
  std::uint16_t Height;

  PureRenderingTarget(std::uint16_t width, std::uint16_t height) : Width(width), Height(height) {}

public:
  std::uint16_t GetWidth() const { return Width; }
  std::uint16_t GetHeight() const { return Height; }

};

template<typename RendererType, typename TargetType>
class RenderingTarget {};

}

#endif
