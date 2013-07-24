#include <Renderers/GLTextureTarget.h>
#include <TextureData.h>

using namespace Uberngine;

RenderingTarget<RendererTypes::OpenGL, RenderTargetType::Texture>::
  RenderingTarget(std::uint16_t width, std::uint16_t height, ColorType ct, void* td) :
  PureRenderingTarget(width, height) {

  GLenum Format = GL_RGB;
  Texture = 0;

  switch (ct) {
  case ColorType::INVALID:
    goto error;
  case ColorType::RGB:
    Format = GL_RGB;
    break;
  case ColorType::RGBA:
    Format = GL_RGBA;
    break;
  case ColorType::GRAY:
    Format = GL_LUMINANCE;
    break;
  case ColorType::GRAY_ALPHA:
    Format = GL_LUMINANCE_ALPHA;
    break;
  case ColorType::ALPHA:
    Format = GL_ALPHA;
    break;
  }

  glGenTextures(1, &Texture);
  glBindTexture(GL_TEXTURE_2D, Texture);
  glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, td);
  if (td) {
    glGenerateMipmap(GL_TEXTURE_2D);
  } 
  // For now only NEAREST_MIPMAP_LINEAR supported
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);

error:
  return;

}

RenderingTarget<RendererTypes::OpenGL, RenderTargetType::Texture>::
  RenderingTarget(std::uint16_t width, std::uint16_t height, ColorType ct) :
  RenderingTarget(width, height, ct, nullptr) {}

RenderingTarget<RendererTypes::OpenGL, RenderTargetType::Texture>::
  ~RenderingTarget() {
  glDeleteTextures(1, &Texture);
}
