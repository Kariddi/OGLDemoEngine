#ifndef __UBERNGINE_RENDERERTRAITS_H__
#define __UBERNGINE_RENDERERTRAITS_H__

#include <cstdint>
#include <Renderers/GLRenderer.h>

namespace Uberngine {

struct OpenGL {};

template<typename RenderType>
struct RendererTraits {};

template<>
struct RendererTraits<OpenGL> {

  typedef GLRenderer RendererType;

};

}

#endif