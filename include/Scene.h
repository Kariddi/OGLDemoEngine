#ifndef __UBERNGINE_SCENE_H__
#define __UBERNGINE_SCENE_H__
#include <vector>
#include <Light.h>

namespace Uberngine {

class Light;
class Camera;
class DynamicsWorld;

class Scene : public Node {

  std::vector<Light*> Lights;
  Camera *Cam;
public:
typedef std::vector<Light*> LightList;
typedef LightList::iterator LightListIt;

  Scene(BaseEngine *e);
  ~Scene();
  //Starts the rendering process of the scene
  void RenderScene();
  //Starts the update of the scene
  void UpdateScene();
  //Sets the camera of the scene
  void SetCamera(Camera *cam) { Cam = cam; }
  //Gets the camera of the scene
  Camera *GetCamera() const { return Cam; }
  //Adds a light to the scene
  void AddLight(Light *lht) { Lights.push_back(lht); }
  //Gets the list of all the lights
  const LightList &GetLights() const { return Lights; }
  void SetGravity(float x, float y, float z); 
  //void AssociateShader(MeshId mid, Shader sh);
};

}
#endif
