#ifndef __UBERNGINE_ENGINE_H__
#define __UBERNGINE_ENGINE_H__
#include <vector>
#include <cstdint>
#include <OGL.h>
#include <Physics/PhysicsManager.h>
#include <OGLSystemFacilities.h>
#include <GUI/GUI.h>
#include <Defs.h>
#include <btBulletDynamicsCommon.h>

/*
  This is the main class of the Engine. Creates the window, renders the scene and manages input.
*/

namespace Uberngine {

class Scene;
class GUI;

class PureEngine {

typedef std::vector<Scene*> SceneList;
typedef SceneList::iterator SceneListIt;

protected:
  PhysicsManager* PhysMan;
  GUI* Gui;
  SceneList Scenes;
  std::uint32_t CurrScene;
  bool DepthTestEnabled;
  float FrameTime;
  float PreviousFrameTime;
  float FrameTimeDelta;
  std::uint16_t Width;
  std::uint16_t Height;
  std::uint8_t Depth;
  PureEngine();
  // Not declared virtual becase no one should know about the
  // existance of this class anyway ...
  ~PureEngine();
public:
  GUI* GetGUI();
  //Creates a new Scene
  Scene* CreateNewScene();
  //Loads a scene
  void LoadScene(std::uint32_t i) { CurrScene = i; }
  float GetFrameTime() { return FrameTime; }
  float GetPreviousFrameTime() { return PreviousFrameTime; }
  float GetFrameTimeDelta() { return FrameTimeDelta; }
  float GetAspectRatio() { return (float) Width / Height; }
  std::uint16_t GetWidth() { return Width; }
  std::uint16_t GetHeight() { return Height; }
  std::uint8_t GetDepth() { return Depth; }
  PhysicsManager* GetPhysicsManager() { return PhysMan; }
  //Updates the scene
  void UpdateScene();
  void RenderGUI() { Gui->RenderGUI(); }
};

template<typename Derived>
class BaseEngine : public PureEngine {
public:
  //Initializes the engine
//  bool Init(const char *window_title, int width, int height, int c_bits, int d_bits, int s_bits, bool fullscreen);
  //Renders the scene
  void RenderScene();
  void StepSingleFrame();
  //Implements the engine Main Loop
  void MainLoop();
  //void AssociateShader(MeshId mid, Shader sh);
};

//Renders the loaded scene
template<typename Derived>
void BaseEngine<Derived>::RenderScene() {
  static_cast<Derived*>(this)->RenderScene();
}

template<typename Derived>
void BaseEngine<Derived>::StepSingleFrame() {
  static_cast<Derived*>(this)->StepSingleFrame();
}

//Implements the engine Main loop
template<typename Derived>
void BaseEngine<Derived>::MainLoop() {
  static_cast<Derived*>(this)->MainLoop();
}

template<PlatformType platform>
class Engine : public BaseEngine<Engine<platform>> { Engine<platform>() {} };

}

#endif
