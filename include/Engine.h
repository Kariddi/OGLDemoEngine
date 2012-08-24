#ifndef __UBERNGINE_ENGINE_H__
#define __UBERNGINE_ENGINE_H__
#include <vector>
#include <OGL.h>
#include <SystemFacilities.h>
#include <SystemFacilitiesFactory.h>
#include <Defs.h>

/*
  This is the main class of the Engine. Creates the window, renders the scene and manages input.
*/

namespace Uberngine {

class Scene;
class Node;
//class SystemFacilities;

class BaseEngine {
typedef std::vector<Scene*> SceneList;
typedef SceneList::iterator SceneListIt;

protected:
  SceneList Scenes;
  int CurrScene;
  bool DepthTestEnabled;
  SystemFacilities *Sys;
  float FrameTime;
  float PreviousFrameTime;
  float FrameTimeDelta;
  BaseEngine();
public:
  virtual ~BaseEngine();
  //Initializes the engine
//  bool Init(const char *window_title, int width, int height, int c_bits, int d_bits, int s_bits, bool fullscreen);
  //Updates the scene
  void UpdateScene();
  //Renders the scene
  void RenderScene();
  //Creates a new Scene
  Scene *CreateNewScene();
  //Loads a scene
  void LoadScene(int i) { CurrScene = i; }
  //Checks if a key is pressed
  bool CheckPressedKey(int key) { return Sys->GetPressedKey(key); }
  //Retrieves the mouse position
  void GetMousePosition(int *x, int *y) { Sys->GetMousePosition(x, y); }
  float GetFrameTime() { return FrameTime; }
  float GetPreviousFrameTime() { return PreviousFrameTime; }
  float GetFrameTimeDelta() { return FrameTimeDelta; }
  void StepSingleFrame();
  //Implements the engine Main Loop
  void MainLoop();
  //void AssociateShader(MeshId mid, Shader sh);
};

template<PlatformType platform>
class Engine : public BaseEngine { Engine<platform>() {} };

}
#endif
