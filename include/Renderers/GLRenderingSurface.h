#ifndef __UBERNGINE_GLRENDERINGSURFACE_H__
#define __UBERNGINE_GLRENDERINGSURFACE_H__

#include <Renderers/RenderingSurface.h>
#include <Renderers/GLRenderer.h>
#include <Defs.h>

namespace Uberngine {

template<typename RendererType>
class Camera;

template<>
class RenderingSurface<RendererTypes::OpenGL> {
};

}

#endif
