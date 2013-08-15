#ifndef __UBERNGINE_GLRENDERPASS_H__
#define __UBERNGINE_GLRENDERPASS_H__

#include <Shader.h>
#include <Node.h>
#include <glm/glm.hpp> //vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale,
#include <glm/gtc/matrix_inverse.hpp>
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
  typedef std::vector<const RenderingTextureTy*> RendTextureListTy;
  typedef std::vector<const RenderingSurfaceTy*> RendSurfaceListTy;
  typedef std::vector<const RendererTy*> RendererListTy;

  // Camera to use for the render pass
//  CameraTy* PassCamera;
  // Shader program to use for the render pass
  //const ShaderTy* PassShader;
  // Input textures for the render pass
  RendTextureListTy InputSurfaces;
  // Output surface for the render pass
  const RenderingSurfaceTy* OutputSurface;
  // Objects that the render pass should draw
  RendererListTy Renderers;
  template<typename RendererType>
  friend class Scene;

  RenderPass() : OutputSurface(nullptr) {}

public:

  void Render(const EngineReal* view_mat, const EngineReal* proj_mat, 
  	const LightList& lights) const {

    OutputSurface->LoadSurface();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    if (OutputSurface->HasDepthAttach()) {
      glClearDepth(1.0f);
      glClear(GL_DEPTH_BUFFER_BIT);
    }

    for (auto Renderer : Renderers) {
      ShaderTy* PassShader = Renderer->GetShader();
      if (PassShader && PassShader->isValid()) {
        GLuint Program = PassShader->getProgram();
        //Setting the shader program
        glUseProgram(PassShader->getProgram());
        //Setting the View transform matrix
        GLuint Loc = glGetUniformLocation(Program, "V");
        glUniformMatrix4fv(Loc, 1, GL_FALSE, view_mat);
       //Setting the Projection matrix
        Loc = glGetUniformLocation(Program, "P");
        glUniformMatrix4fv(Loc, 1, GL_FALSE, proj_mat);
      
        glm::mat4 View = glm::make_mat4(view_mat);
        const LightList &LL = lights; 
        int light_num = LL.size();
        Loc = glGetUniformLocation(Program, "LightNum");
        glUniform1i(Loc, light_num);
        //Setting the lights parameters
        for (int i = 0; i < light_num; ++i) {
          std::stringstream ss;
          ss << "L[" << i << "]";
          Loc = glGetUniformLocation(Program, (ss.str() + ".position").c_str());
          glm::vec4 Pos = glm::make_vec4(LL[i]->GetPosition());
          glUniform3fv(Loc, 1, glm::value_ptr(Pos*View));
          Loc = glGetUniformLocation(Program, (ss.str() + ".dir").c_str());
          glm::vec4 Dir = glm::make_vec4(LL[i]->GetDirection());
          glUniform3fv(Loc, 1, glm::value_ptr(Dir*View));
          Loc = glGetUniformLocation(Program, (ss.str() + ".color").c_str());
          glUniform3fv(Loc, 1, LL[i]->GetColor());
          Loc = glGetUniformLocation(Program, (ss.str() + ".type").c_str());
          glUniform1i(Loc, static_cast<GLint>(LL[i]->GetLightType()));
        }
        //Set if lighting is being used
        Loc = glGetUniformLocation(Program, "is_lighted");
        if (light_num > 0)
          glUniform1i(Loc, 1);
        else
          glUniform1i(Loc, 0);
   
        std::uint8_t tex_idx = 0;
        for (auto Texture : InputSurfaces) {
          std::stringstream ss;
          ss << "Textures[" << tex_idx << "]";
          Loc = glGetUniformLocation(Program, ss.str().c_str());
          glUniform1i(Loc, tex_idx);
          glActiveTexture(GL_TEXTURE0 + tex_idx);
          glBindTexture(GL_TEXTURE_2D, Texture->GetGLObject());
          ++tex_idx;
        }
   
        Renderer->Render(view_mat, proj_mat, lights, tex_idx);
      }
    }

  }

  void SetOutputSurface(const RenderingSurfaceTy* out_surface) { OutputSurface = out_surface; }
  void AddInputSurface(const RenderingTextureTy* tex_surface) { InputSurfaces.push_back(tex_surface); }
  void AddObject(const RendererTy* object) { Renderers.push_back(object); }
  void AddObjects(const RendererListTy& render_list) { 
    Renderers.clear(); 
    Renderers.insert(Renderers.begin(), render_list.begin(), render_list.end()); 
  }

};

}

#endif
