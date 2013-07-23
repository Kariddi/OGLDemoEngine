#ifndef __UBERNGINE_RENDERPASS_H__
#define __UBERNGINE_RENDERPASS_H__

#include <Shader.h>
#include <Node.h>

namespace Uberngine {

template<typename RendererType>
class RenderPass {

  typedef Shader<RendererType> ShaderTy;
  typedef Node<RendererType> NodeTy;

  ShaderTy* Shad;
  typename NodeTy::NodeList Nodes;

public:

  void Render() {}
  inline void AddNode(NodeTy* node) { Nodes.push_back(); } 

};

}

#endif
