#ifndef __UBERNGINE_ENGINETRAITS_H__
#define __UBERNGINE_ENGINETRAITS_H__

#include <cstdint>
#include <EngineTraits.h>
#include <Defs.h>

namespace Uberngine {

template<PlatformType pt>
class Engine;
template<typename RendererType>
class Node;
template<typename RendererType>
class Scene;
template<typename RendererType>
class Camera;
template<typename RendererType>
class Shader;

template<typename EngineType>
struct EngineTraits {};

template<>
struct EngineTraits<Engine<UBE_GLFW>> {
  typedef RendererTypes::OpenGL RendererType;
  typedef Node<RendererType> NodeTy;
  typedef Scene<RendererType> SceneTy;
  typedef Camera<RendererType> CameraTy;
  typedef Shader<RendererType> ShaderTy;

};

template<>
struct EngineTraits<Engine<UBE_IOS>> {
  typedef RendererTypes::OpenGL RendererType;
  typedef Node<RendererType> NodeTy;
  typedef Scene<RendererType> SceneTy;
  typedef Camera<RendererType> CameraTy;
  typedef Shader<RendererType> ShaderTy;

};

}

#endif
