#ifndef __UBERNGINE_NODE_H__
#define __UBERNGINE_NODE_H__
#include <vector>
#include <algorithm>
#include <glm/glm.hpp> //vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale,
#include <glm/gtc/type_ptr.hpp> //value_ptr
#include <glm/gtc/quaternion.hpp>
#include <Renderers/GLRenderer.h>
#include <Physics/RigidBody.h>

namespace Uberngine {

template<typename RendererType>
class Scene;
class PhysicsManager;
struct Mesh;

class PureNode {

 glm::mat4 GLTransform;
  static glm::quat IdQuat;

protected:
  RigidBody* Body;
  glm::mat4 Comulative; 

  PureNode();
  ~PureNode();
public:
 inline const glm::mat4& GetComulative() const { return Comulative; }
  //Adds a child node to this node
  void SetRigidBody(RigidBody* rb);
  RigidBody* GetRigidBody() const { return Body; }
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
};

template<typename RendererType>
class Node : public PureNode {
public:
  typedef std::vector<Node*> NodeList;
  typedef typename NodeList::iterator NodeListIt;

private:
  typedef Renderer<RendererType> NodeRendererTy;

  Node<RendererType>* Parent;
  Scene<RendererType>* CurrentScene;
  NodeRendererTy Rend;
  NodeList Children;

  inline const NodeRendererTy& GetRenderer() const { return Rend; }
  inline void SetScene(Scene<RendererType>* sc) { CurrentScene = sc; }
  inline Scene<RendererType>* GetScene() const { return CurrentScene; }

  void GlobalInitialize();
  void GlobalUpdate();

  friend class Scene<RendererType>;
public:
  Node() : PureNode(), Parent(nullptr), CurrentScene(nullptr), Rend(Comulative) {}
  ~Node() { SetMesh(nullptr); }
 //Attaches a mesh to this Node
  void SetMesh(Mesh* mesh);
  //Attaches a shader to this Node
  void SetShader(Shader<RendererType>* shader);
  void AddChildNode(Node<RendererType>* node);
  //These must be used from inheriting classes to extend functionality
  //Update can be used to update the position of the node , doing physical calculations ... etc
  //virtual void Update() {};
  //Initialize can be used to do some specific initialization when a scene is loaded
  //virtual void Initialize() {};
};

template<typename RendererType>
void Node<RendererType>::AddChildNode(Node<RendererType>* node) {
  if (node->Parent != nullptr) {
    NodeList &OldPChildren = node->Parent->Children;
    OldPChildren.erase(std::find(OldPChildren.begin(), OldPChildren.end(), node));
  }
  node->Parent = this;
  //node->DynWorld = nullptr;
  Children.push_back(node);
}

template<typename RendererType>
void Node<RendererType>::SetMesh(Mesh* mesh) {
  Rend.SetMesh(mesh);
}

template<typename RendererType>
void Node<RendererType>::SetShader(Shader<RendererType>* shader) {
  Rend.SetShader(shader);
}


template<typename RendererType>
void Node<RendererType>::GlobalInitialize() {
  //Initialize();
  for (auto I : Children)
    I->GlobalInitialize();
}

template<typename RendererType>
void Node<RendererType>::GlobalUpdate() {
  //std::cout << "Global Updating Node" << std::endl;
  //Update();
  if (Body != nullptr) {
    float trans[16];
    Body->GetPosition(trans);
    Comulative = glm::make_mat4(trans);
  } else {    
  //Multiplying the Model matrix with the Comulative matrix of the parent
  //Child nodes move together with parent nodes (if the parent node moves the child node moves with him)
    if (Parent)
      Comulative = Parent->Comulative*glm::make_mat4(GetTransform());
    else
      Comulative = glm::mat4(1.0f);
  }

  for (auto I : Children)
    I->GlobalUpdate();
}

}

#endif
