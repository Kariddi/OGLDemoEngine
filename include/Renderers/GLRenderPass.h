#ifndef __UBERNGINE_GLRENDERPASS_H__
#define __UBERNGINE_GLRENDERPASS_H__

#include <Shader.h>
#include <Node.h>
#include <Renderers/GLRenderingSurface.h>
#include <Renderers/RenderPass.h>
#include <vector>

namespace Uberngine {

template<>
class RenderPass<RendererTypes::OpenGL> {
  typedef RendererTypes::OpenGL RendererType;
  typedef Camera<RendererType> CameraTy;
  typedef Shader<RendererType> ShaderTy;
  typedef Renderer<RendererType> RendererTy;
  typedef RenderingSurface<RendererType> RenderingSurfaceTy;
  typedef std::vector<RenderingSurfaceTy*> RendSurfaceListTy;
  typedef std::vector<RendererTy*> RendererListTy;

  CameraTy* PassCamera;
  ShaderTy* PassShader;
  RendSurfaceListTy InputSurfaces;
  RendSurfaceListTy OutputSurfaces;
  RendererListTy Renderers;

public:

  void Render() {}

};

}

#endif
