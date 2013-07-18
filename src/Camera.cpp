#include <Camera.h>
#include <Scene.h>
#include <cmath>

using namespace Uberngine;

Camera::Camera(Scene *parent) : Node(parent->GetRootNode()) ,CT(PERSPECTIVE), CamViewMatrix(1.0f), 
                                             CamProjMatrix(1.0f) {}

Camera::~Camera() {}

void Camera::SetPerspectiveCamera(float fov, float aspect, float near, float far) {

 CT = PERSPECTIVE;

 CamProjMatrix = glm::perspective(fov, aspect, near, far);
 CamPVMatrix = CamViewMatrix*CamProjMatrix;

}


void Camera::LookAt(float eyex, float eyey, float eyez, 
                    float centerx, float centery, float centerz, 
                    float upx, float upy, float upz) {

  CamViewMatrix = glm::lookAt(glm::vec3(eyex, eyey, eyez),
                              glm::vec3(centerx, centery, centerz),
                              glm::vec3(upx, upy, upz));
  CamPVMatrix = CamProjMatrix*CamViewMatrix;
}

