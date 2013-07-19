#ifndef __UBERNGINE_CAMERA_H__
#define __UBERNGINE_CAMERA_H__
#include <vector>
#include <Node.h>
/*
  This class represents a Camera object that can be added to a Scene
*/
namespace Uberngine {

class Scene;

class Camera : public Node {
  enum _CameraType { ORTOGRAPHIC, PERSPECTIVE };
  _CameraType CT;
  glm::mat4 CamViewMatrix;
  glm::mat4 CamProjMatrix;
  glm::mat4 CamPVMatrix;
public:
typedef _CameraType CameraType;
  Camera();
  ~Camera();
  void SetPerspectiveCamera(float fov, float aspect, float near, float far);
  void LookAt(float eyex, float eyey, float eyez, 
              float centerx, float centery, float centerz, 
              float upx, float upy, float upz);
  const float *GetViewMatrix() const { return glm::value_ptr(CamViewMatrix); }
  const float *GetProjMatrix() const { return glm::value_ptr(CamProjMatrix); }
  const float *GetPVMatrix() const { return glm::value_ptr(CamPVMatrix); }
};

}

#endif
