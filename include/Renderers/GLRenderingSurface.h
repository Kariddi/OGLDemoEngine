#ifndef __UBERNGINE_GLRENDERINGSURFACE_H__
#define __UBERNGINE_GLRENDERINGSURFACE_H__

#include <Renderers/RenderingSurface.h>
#include <Renderers/GLRenderer.h>
#include <Defs.h>

namespace Uberngine {

template<>
class RenderingSurface<RendererTypes::OpenGL> {

protected:
  GLuint Framebuffer;
  const GLRenderingTarget* ColorAttach;
  const GLRenderingTarget* DepthAttach;
  const GLRenderingTarget* StencilAttach;
  bool HasDoubleBuffer;

public:
  RenderingSurface(const GLRenderingTarget* color_attach = nullptr, 
                   const GLRenderingTarget* depth_attach = nullptr, 
                   const GLRenderingTarget* stencil_attach = nullptr,
                   bool has_doublebuffer = true);

  GLuint GetGLObject() const { return Framebuffer; }
  inline const GLRenderingTarget* GetColorAttach() const { return ColorAttach; }
  inline const GLRenderingTarget* GetDepthAttach() const { return DepthAttach; }
  inline const GLRenderingTarget* GetStencilAttach() const { return StencilAttach; }
  inline bool HasColorAttach() const { return ColorAttach != nullptr; } 
  inline bool HasDepthAttach() const { return DepthAttach != nullptr; } 
  inline bool HasStencilAttach() const { return StencilAttach != nullptr; } 
  void LoadSurface() const;

};

}

#endif
