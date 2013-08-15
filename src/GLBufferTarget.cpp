#include <Renderers/GLBufferTarget.h>

using namespace Uberngine;

RenderingTarget<RendererTypes::OpenGL, RenderTargetType::Buffer>::
  RenderingTarget(std::uint16_t width, std::uint16_t height, ColorType ct) :
  GLRenderingTarget(width, height, GLRenderingTarget::TargetType::Buffer),
  NeedsDelete(true) {

  GLenum Format = GL_RGB;

  switch (ct) {
  default:
  case ColorType::INVALID:
    goto error;
  case ColorType::RGB:
    Format = GL_RGB;
    break;
  case ColorType::RGBA:
    Format = GL_RGBA;
    break;
  case ColorType::DEPTH:
    Format = GL_DEPTH_COMPONENT;
    break;
  }

  glGenRenderbuffers(1, &GLObject);
  glBindRenderbuffer(GL_RENDERBUFFER, GLObject);
  glRenderbufferStorage(GL_RENDERBUFFER, Format, Width, Height);

  glBindRenderbuffer(GL_RENDERBUFFER, 0);

error:
  return;

}

RenderingTarget<RendererTypes::OpenGL, RenderTargetType::Buffer>::
  RenderingTarget(std::uint16_t width, std::uint16_t height, GLuint gl_object) :
  GLRenderingTarget(width, height, GLRenderingTarget::TargetType::Buffer, gl_object),
  NeedsDelete(false) {}

RenderingTarget<RendererTypes::OpenGL, RenderTargetType::Buffer>::
  ~RenderingTarget() {
  if (NeedsDelete)
    glDeleteRenderbuffers(1, &GLObject);
}
