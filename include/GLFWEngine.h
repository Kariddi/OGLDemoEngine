#ifndef __UBERNGINE_ENGINE_UBE_GLFW_H__
#define __UBERNGINE_ENGINE_UBE_GLFW_H__
//#include <GL/internal.h>
#include <GLFW/glfw3.h>
#include <GLFWSystemFacilities.h>
#include <Renderers/GLRenderingSurface.h>
#include <Renderers/GLTarget.h>
#include <Engine.h>
#include <Scene.h>
#include <vector>

namespace Uberngine {

template<>
class Engine<UBE_GLFW> : public BaseEngine<Engine<UBE_GLFW>> {
  typedef RenderingTarget<RendererTypes::OpenGL, RenderTargetType::Default> DefaultRenderingTargetTy;
  typedef RenderingTarget<RendererTypes::OpenGL, RenderTargetType::Texture> TextureRenderingTargetTy;
  typedef RenderingTarget<RendererTypes::OpenGL, RenderTargetType::Buffer> BufferRenderingTargetTy;
  typedef std::vector<const DefaultRenderingTargetTy*> DefaultRenderingTargetListTy;
  typedef std::vector<const TextureRenderingTargetTy*> TextureRenderingTargetListTy;
  typedef std::vector<const BufferRenderingTargetTy*> BufferRenderingTargetListTy;

  GUI* Gui;
  GLFWSystemFacilities Sys;
  RenderingSurface<RendererTypes::OpenGL>* DefaultSurface;
  DefaultRenderingTargetListTy DefaultRenderingTargets;
  TextureRenderingTargetListTy TextureRenderingTargets;
  BufferRenderingTargetListTy BufferRenderingTargets;

public:
  typedef GLFWSystemFacilities::KeyCallbackTy KeyCallbackTy;
  Engine();
  ~Engine();

  bool Init(const char *window_title, int width, int height, int c_bits, 
                      int d_bits, int s_bits, bool fullscreen);
  Scene<RendererTypes::OpenGL>* CreateNewScene();
  template<typename RenderTargetType>
  RenderingTarget<RendererTypes::OpenGL, RenderTargetType>* CreateNewRenderingTarget(std::uint16_t width, std::uint16_t height, ColorType ct);
  void SetPressedKeyCallback(KeyCallbackTy kc);
  bool CheckPressedKey(int key) { return Sys.GetPressedKey(key); }
  //Retrieves the mouse position
  void GetMousePosition(int* x, int* y) { Sys.GetMousePosition(x, y); }
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

template<typename RenderTargetTy>
RenderingTarget<RendererTypes::OpenGL, RenderTargetTy>* Engine<UBE_GLFW>::CreateNewRenderingTarget(std::uint16_t width, std::uint16_t height, ColorType ct) {

  static_assert((std::is_same<RenderTargetTy, RenderTargetType::Texture>::value || 
                std::is_same<RenderTargetTy, RenderTargetType::Buffer>::value), 
                "Calling CreateNewRenderingTarget with unsupported target type");

  if (std::is_same<RenderTargetTy, RenderTargetType::Texture>()) {
    RenderingTarget<RendererTypes::OpenGL, RenderTargetTy>* NewTextureTarget = 
      new RenderingTarget<RendererTypes::OpenGL, RenderTargetTy>(width, height, ct, NULL);
    TextureRenderingTargets.push_back(NewTextureTarget);
    return NewTextureTarget;
  } else {
    RenderingTarget<RendererTypes::OpenGL, RenderTargetTy>* NewBufferTarget = 
      new RenderingTarget<RendererTypes::OpenGL, RenderTargetTy>(width, height, ct);
    BufferRenderingTargets.push_back(NewBufferTarget);
    return NewBufferTarget;
  }

  return NULL;
}

}

#endif
