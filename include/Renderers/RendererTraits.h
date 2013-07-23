#ifndef __UBERNGINE_RENDERERTRAITS_H__
#define __UBERNGINE_RENDERERTRAITS_H__

#include <cstdint>
#include <Renderers/GLRenderer.h>
#include <Defs.h>

namespace Uberngine {

template<typename RenderType>
struct RendererTraits {};

template<>
struct RendererTraits<RendererTypes::OpenGL> {

  typedef GLRenderer RendererObjType;

};

}

#endif
