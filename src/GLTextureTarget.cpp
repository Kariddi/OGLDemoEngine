#include <Renderers/GLTextureTarget.h>

using namespace Uberngine;

RenderingTarget<RendererTypes::OpenGL, RenderTargetType::Texture>::
  RenderingTarget(std::uint16_t width, std::uint16_t height, ColorType ct, void* td) :
  GLRenderingTarget(width, height, GLRenderingTarget::TargetType::Texture) {

  GLenum Format = GL_RGB;
  GLuint Type = GL_UNSIGNED_BYTE;

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
  case ColorType::DEPTH:
    Format = GL_DEPTH_COMPONENT;
    Type = GL_FLOAT;
    break;
  }

  glGenTextures(1, &GLObject);
  glBindTexture(GL_TEXTURE_2D, GLObject);
  glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, Type, td);
  if (td) {
    glGenerateMipmap(GL_TEXTURE_2D);
  } 
  // For now only NEAREST_MIPMAP_LINEAR supported
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

  // Setting texture paramenters for Depth textures
  if (ct == ColorType::DEPTH) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //  glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  }

  glBindTexture(GL_TEXTURE_2D, 0);

error:
  return;

}

RenderingTarget<RendererTypes::OpenGL, RenderTargetType::Texture>::
  RenderingTarget(std::uint16_t width, std::uint16_t height, ColorType ct) :
  RenderingTarget(width, height, ct, nullptr) {}

RenderingTarget<RendererTypes::OpenGL, RenderTargetType::Texture>::
  ~RenderingTarget() {
  glDeleteTextures(1, &GLObject);
}
