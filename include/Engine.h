#ifndef __UBERNGINE_ENGINE_H__
#define __UBERNGINE_ENGINE_H__
#include <vector>
#include <cstdint>
#include <OGL.h>
#include <EngineTraits.h>
#include <Node.h>
#include <Physics/PhysicsManager.h>
#include <OGLSystemFacilities.h>
#include <GUI/GUI.h>
#include <Defs.h>
#include <btBulletDynamicsCommon.h>

/*
  This is the main class of the Engine. Creates the window, renders the scene and manages input.
*/

namespace Uberngine {

template<typename RendererType>
class Scene;
class GUI;

class PureEngine {

protected:
  PhysicsManager PhysMan;
  std::uint32_t CurrScene;
  bool DepthTestEnabled;
  float FrameTime;
  float PreviousFrameTime;
  float FrameTimeDelta;
  std::uint16_t Width;
  std::uint16_t Height;
  std::uint8_t Depth;
  PureEngine() : CurrScene(0), DepthTestEnabled(false),
                           FrameTime(0.0f),
                           PreviousFrameTime(0.0f), 
                           FrameTimeDelta(0.0f),
                           Width(0), Height(0), Depth(0)  {}
  // Not declared virtual becase no one should know about the
  // existance of this class anyway ...
  ~PureEngine() {}
public:
  //Creates a new Scene
  //Loads a scene
  void LoadScene(std::uint32_t i) { CurrScene = i; }
  float GetFrameTime() { return FrameTime; }
  float GetPreviousFrameTime() { return PreviousFrameTime; }
  float GetFrameTimeDelta() { return FrameTimeDelta; }
  float GetAspectRatio() { return (float) Width / Height; }
  std::uint16_t GetWidth() { return Width; }
  std::uint16_t GetHeight() { return Height; }
  std::uint8_t GetDepth() { return Depth; }
  PhysicsManager* GetPhysicsManager() { return &PhysMan; }
  //Updates the scene
  //void UpdateScene();
};

template<typename Derived>
class BaseEngine : public PureEngine {
  typedef typename EngineTraits<Derived>::RendererType RendererType;
  typedef std::vector<Scene<RendererType>*> SceneList;
  typedef typename SceneList::iterator SceneListIt;

protected:
  SceneList Scenes;
  ~BaseEngine() {
    for (auto I : Scenes) {
      delete I;
    }
  }
public:
  typedef typename EngineTraits<Derived>::NodeTy NodeTy;

  Scene<RendererType>* CreateNewScene();
  //Renders the scene
  void RenderScene();
  void StepSingleFrame();
  //Implements the engine Main Loop
  void MainLoop();
//Updates the currently loaded scene
  void UpdateScene();

};

//Creates a new Scene
template<typename Derived>
Scene<typename BaseEngine<Derived>::RendererType>* BaseEngine<Derived>::CreateNewScene() {

  Scene<typename BaseEngine<Derived>::RendererType>* NewScene = 
    new Scene<typename BaseEngine<Derived>::RendererType>(&PhysMan);
  Scenes.push_back(NewScene);

  return NewScene;
}

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

template<typename Derived>
void BaseEngine<Derived>::UpdateScene() {
  Scenes[CurrScene]->UpdateScene(GetFrameTimeDelta());
}

template<PlatformType platform>
class Engine : public BaseEngine<Engine<platform>> { Engine<platform>() {} };

}

#endif
