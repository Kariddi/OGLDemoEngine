#ifndef __UBERNGINE_SCENE_H__
#define __UBERNGINE_SCENE_H__
#include <vector>
#include <Light.h>

namespace Uberngine {

class Camera;
class PhysicsManager;
class Node;

class Scene {

  typedef std::vector<Node*> NodeList;
  typedef NodeList::iterator NodeListIt;

  LightList Lights;
  Node* RootNode;
  Camera* Cam;
  PhysicsManager* PhysMan;
  DynamicsWorld* DynWorld;

  Scene(PhysicsManager* p_manager);
  friend class PureEngine;

public:

  ~Scene();
  //Starts the rendering process of the scene
  void RenderScene();
  //Starts the update of the scene
  void UpdateScene(float frame_time_delta);
  //Sets the camera of the scene
  void SetCamera(Camera* cam) { Cam = cam; }
  //Gets the camera of the scene
  Camera* GetCamera() const { return Cam; }
  //Adds a light to the scene
  void AddLight(Light* lht) { Lights.push_back(lht); }
  //Gets the list of all the lights
  const LightList& GetLights() const { return Lights; }
  void SetGravity(float x, float y, float z);
  Node* GetRootNode() const { return RootNode; }
  DynamicsWorld* GetDynamicsWorld() { return DynWorld; }
  //void AssociateShader(MeshId mid, Shader sh);
};

}
#endif
