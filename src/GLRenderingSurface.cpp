#include <Renderers/GLRenderingSurface.h>
#include <limits>

using namespace Uberngine;

RenderingSurface<RendererTypes::OpenGL>::RenderingSurface(const GLRenderingTarget* color_attach, 
  const GLRenderingTarget* depth_attach, 
  const GLRenderingTarget* stencil_attach, bool has_doublebuffer) : ColorAttach(color_attach), DepthAttach(depth_attach),
  StencilAttach(stencil_attach), HasDoubleBuffer(has_doublebuffer) {

  Framebuffer = 0; 

  if ((color_attach == nullptr || color_attach->GetType() == GLRenderingTarget::TargetType::Default) && 
      (depth_attach == nullptr || depth_attach->GetType() == GLRenderingTarget::TargetType::Default) &&
      (stencil_attach == nullptr || stencil_attach->GetType() == GLRenderingTarget::TargetType::Default))
    return;

  glGenFramebuffers(1, &Framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);

  if (color_attach != nullptr) {
    GLRenderingTarget::TargetType Type = color_attach->GetType();
    if (Type == GLRenderingTarget::TargetType::Texture) {
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attach->GetGLObject(), 0);
    } else {
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, color_attach->GetGLObject()); 
    }
  }

  if (depth_attach != nullptr) {
    GLRenderingTarget::TargetType Type = depth_attach->GetType();
    if (Type == GLRenderingTarget::TargetType::Texture) {
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_attach->GetGLObject(), 0);
    } else {
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_attach->GetGLObject()); 
    }
  }

  if (stencil_attach != nullptr) {
    GLRenderingTarget::TargetType Type = stencil_attach->GetType();
    if (Type == GLRenderingTarget::TargetType::Texture) {
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, stencil_attach->GetGLObject(), 0);
    } else {
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencil_attach->GetGLObject()); 
    }
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void RenderingSurface<RendererTypes::OpenGL>::LoadSurface() const {

  glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);

  // This is the default framebuffer
  if ((ColorAttach != nullptr && ColorAttach->GetType() == GLRenderingTarget::TargetType::Default) &&
      (DepthAttach == nullptr || DepthAttach->GetType() == GLRenderingTarget::TargetType::Default) &&
      (StencilAttach == nullptr || StencilAttach->GetType() == GLRenderingTarget::TargetType::Default)) {
      
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    if (DepthAttach != nullptr) {
      glDepthMask(GL_TRUE);
      glEnable(GL_DEPTH_TEST);
    }
    if (StencilAttach != nullptr) {
      glStencilMask(std::numeric_limits<GLuint>::max());
      glEnable(GL_STENCIL_TEST);
    }
  } else {
    if (ColorAttach == nullptr) {
      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    } else {
      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }

    if (DepthAttach == nullptr) {
      glDepthMask(GL_FALSE);
      glDisable(GL_DEPTH_TEST);
    } else {
      glDepthMask(GL_TRUE);
      glEnable(GL_DEPTH_TEST); 
    }

    if (StencilAttach == nullptr) {
      glStencilMask(0);
      glDisable(GL_STENCIL_TEST);
    } else {
      glStencilMask(std::numeric_limits<GLuint>::max());
      glEnable(GL_STENCIL_TEST);
    }
  }
}
