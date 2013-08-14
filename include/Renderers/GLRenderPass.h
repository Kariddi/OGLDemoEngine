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
  typedef RenderingTarget<RendererType, RenderTargetType::Texture> RenderingTextureTy;
  typedef RenderingSurface<RendererType> RenderingSurfaceTy;
  typedef std::vector<RenderingTextureTy*> RendTextureListTy;
  typedef std::vector<RenderingSurfaceTy*> RendSurfaceListTy;
  typedef std::vector<RendererTy*> RendererListTy;

  // Camera to use for the render pass
//  CameraTy* PassCamera;
  // Shader program to use for the render pass
  ShaderTy* PassShader;
  // Input textures for the render pass
  RendTextureListTy InputSurfaces;
  // Output surface for the render pass
  RenderingSurfaceTy* OutputSurface;
  // Objects that the render pass should draw
  RendererListTy Renderers;

public:

  void Render(const EngineReal* view_mat, const EngineReal* proj_mat, 
  	const LightList& lights) const {

    for (auto Renderer : Renderers) {
      Renderer->Render(view_mat, proj_mat, lights, 0);
    }

  }

};

}

#endif
