#ifndef __UBERNGINE_NODE_H__
#define __UBERNGINE_NODE_H__
#include <vector>
#include <OGL.h>
#include <glm/glm.hpp> //vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale,
#include <glm/gtc/type_ptr.hpp> //value_ptr
#include <glm/gtc/quaternion.hpp>
#include <Mesh.h>
#include <Traits.h>

namespace Uberngine {

class BaseEngine;
class Shader;
class Scene;

class Node {

struct PartRenderCtx {
  GLuint PartVAO;
  GLuint VBO;
};

typedef Mesh<Traits::MeshVertexType, Traits::MeshIDXType> _MeshTy;
typedef GLMeshTraits<Traits::MeshVertexType, Traits::MeshIDXType> GLTraits;
  _MeshTy *NodeMesh;
  PartRenderCtx *RenderCTXs;
  GLuint VertVBO;
  GLuint *Textures;
  //GLuint Samplers[3];
  Shader *ShaderProg;
  Node *Parent;
  glm::mat4 Comulative;
  glm::mat4 GLTransform;
  static glm::quat IdQuat;
  //bool Hooked;

protected:
typedef std::vector<Node*> NodeList;
typedef NodeList::iterator NodeListIt;

  NodeList Children;
  BaseEngine &Eng;
  void GlobalInitialize();
  void Render(Scene *scene);
  void GlobalUpdate();

public:
typedef _MeshTy MeshTy;
  Node(BaseEngine *e, Node *parent);
  virtual ~Node();
  //For physics use ...
  //void AttachRigidBody();
  //Adds a child node to this node
  void AddChildNode(Node *node);
  //Attaches a mesh to this Node
  void SetMesh(MeshTy *mesh);
  //Attaches a shader to this Node
  void SetShader(Shader *shader);
  //Returns an array representing the Transform of the node
  const float *GetTransform() const { return glm::value_ptr(GLTransform); }
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
