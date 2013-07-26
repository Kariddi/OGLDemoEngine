#ifndef __UBERNGINE_IOSENGINE_H__
#define __UBERNGINE_IOSENGINE_H__
#include <Engine.h>
#include <Renderers/GLBufferTarget.h>
#include <Renderers/GLDefaultTarget.h>
#include <Renderers/GLTextureTarget.h>
#include <EAGLSystemFacilities.h>
#import <UIKit/UIView.h>
#import <QuartzCore/CAEAGLLayer.h>
#include <vector>

namespace Uberngine {

template<>
class Engine<UBE_IOS> : public BaseEngine<Engine<UBE_IOS>> {
  typedef RenderingTarget<RendererTypes::OpenGL, RenderTargetType::Default> DefaultRenderingTargetTy;
  typedef RenderingTarget<RendererTypes::OpenGL, RenderTargetType::Texture> TextureRenderingTargetTy;
  typedef RenderingTarget<RendererTypes::OpenGL, RenderTargetType::Buffer> BufferRenderingTargetTy;
  typedef std::vector<const DefaultRenderingTargetTy*> DefaultRenderingTargetListTy;
  typedef std::vector<const TextureRenderingTargetTy*> TextureRenderingTargetListTy;
  typedef std::vector<const BufferRenderingTargetTy*> BufferRenderingTargetListTy;

  RenderingSurface<RendererTypes::OpenGL>* DefaultSurface;
  BufferRenderingTargetListTy BufferRenderingTargets;
  UIView *IosView;
  GUI* Gui;
  EAGLSystemFacilities Sys;
public:
  Engine<UBE_IOS>(UIView* layer, CADisplayLink* display_link) : IosView(layer),
    Gui(nullptr), Sys((CAEAGLLayer*)IosView.layer, display_link) {}
  bool Init(int width, int height, int c_bits, int d_bits, int s_bits) {
    bool ret_val = false;
    ret_val = this->Sys.CreateAndSetRenderContext(&width, &height, c_bits, d_bits, s_bits, false);
    const BufferRenderingTargetTy* ColorTarget = new BufferRenderingTargetTy(width, height, this->Sys.GetColorRenderbuffer());
    const BufferRenderingTargetTy* DepthTarget = nullptr;
      
    BufferRenderingTargets.push_back(ColorTarget);
    if (ret_val) {
      this->Width = width;
      this->Height = height;
      this->Depth = c_bits;
      if (d_bits != 0) {
        DepthTarget = new BufferRenderingTargetTy(width, height, this->Sys.GetDepthRenderbuffer());
        BufferRenderingTargets.push_back(DepthTarget);
        DepthTestEnabled = true;
      }
    }
    
    DefaultSurface = new RenderingSurface<RendererTypes::OpenGL>(ColorTarget, DepthTarget);
    this->Sys.SetDrawFramebufferObject(DefaultSurface->GetGLObject());
    return ret_val;
  }
  
  ~Engine() {
    if (DefaultSurface != nullptr)
      delete DefaultSurface;
      
    for (auto S : BufferRenderingTargets)
      delete S;
  }

  GLuint GetFramebuffer() const { return this->Sys.GetDrawFramebufferObject(); }
  GLuint GetColorRenderbuffer() const { return this->Sys.GetColorRenderbuffer(); }
  GLuint GetDepthRenderbuffer() const { return this->Sys.GetDepthRenderbuffer(); }
  EAGLSystemFacilities *GetSysFacilities() { return static_cast<EAGLSystemFacilities*>(&this->Sys); }
    //Renders the loaded scene
  void RenderScene();

  void StepSingleFrame();

  //Implements the engine Main loop
  void MainLoop();

  GUI* GetGUI() {
    if (Gui == nullptr)
      Gui = new GUI;
    return Gui;
  }

  void RenderGUI() { Gui->RenderGUI(); }
};

}
#endif
