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

public:
  typedef GLFWSystemFacilities::KeyCallbackTy KeyCallbackTy;
  Engine();
  ~Engine();

  bool Init(const char *window_title, int width, int height, int c_bits, 
                      int d_bits, int s_bits, bool fullscreen);
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

}

#endif
