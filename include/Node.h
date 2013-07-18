#ifndef __UBERNGINE_NODE_H__
#define __UBERNGINE_NODE_H__
#include <vector>
#include <glm/glm.hpp> //vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale,
#include <glm/gtc/type_ptr.hpp> //value_ptr
#include <glm/gtc/quaternion.hpp>
#include <Renderers/GLRenderer.h>
#include <Renderers/RendererTraits.h>
#include <Physics/RigidBody.h>

namespace Uberngine {

//class Shader;
class Scene;
//class RigidBody;
class PhysicsManager;
class Mesh;

class Node {

  Node* Parent;
  Scene* CurrentScene;
  RigidBody* Body;
  glm::mat4 Comulative;
  glm::mat4 GLTransform;
  static glm::quat IdQuat;

  RendererTraits<OpenGL>::RendererType Rend;

  friend class Scene;

protected:
typedef std::vector<Node*> NodeList;
typedef NodeList::iterator NodeListIt;

  NodeList Children;
 
  void GlobalInitialize();
  void Render(Scene* scene);
  void GlobalUpdate();
  //void SetDynamicsWorld(DynamicsWorld *d_world) { DynWorld = d_world; }
  Node();
  virtual ~Node();

public:
//typedef _MeshTy MeshTy;
  Node(Node* parent);
  Node(Scene* parent);
  //For physics use ...
  //void AttachRigidBody();
  //Adds a child node to this node
  void AddChildNode(Node* node);
  //Attaches a mesh to this Node
  void SetMesh(Mesh* mesh);
  //Attaches a shader to this Node
  void SetShader(Shader* shader);
  void SetRigidBody(RigidBody* rb);
  //Returns an array representing the Transform of the node
  const float* GetTransform() const { return glm::value_ptr(GLTransform); }
  //Returns the Kinematic status of the node
  bool IsKinematic() {
    if (Body != nullptr)
      return Body->IsKinematic();
    return true;
  }
  //Sets the Kinematic status of the object
  void SetKinematic(bool kin) {
    if (Body != nullptr) {
      Body->SetKinematic(kin);
    }
  }
  //Sets the transform of the node. The first 3 parameters are the translation of the node
  //the angle is the amount of rotation in radians and the last 3 parameters are the axis of rotation
  void SetTransform(float transX, float transY, float transZ, float angle, float axisX, float axisY, float axisZ);
  //These must be used from inheriting classes to extend functionality
  //Update can be used to update the position of the node , doing physical calculations ... etc
  virtual void Update() {};
  //Initialize can be used to do some specific initialization when a scene is loaded
  virtual void Initialize() {};
};

}

#endif
