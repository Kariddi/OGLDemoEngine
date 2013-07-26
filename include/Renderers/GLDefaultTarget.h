#ifndef __UBERNGINE_GLDEFAULTTARGET_H__
#define __UBERNGINE_GLDEFAULTTARGET_H__

#include <Renderers/RenderingTarget.h>
#include <Renderers/GLTarget.h>
#include <Defs.h>
#include <OGL.h>
#include <cstdint>

namespace Uberngine {

template<>
class RenderingTarget<RendererTypes::OpenGL, RenderTargetType::Default> : public GLRenderingTarget {

public:
  RenderingTarget(std::uint16_t width, std::uint16_t height) : 
    GLRenderingTarget(width, height, GLRenderingTarget::TargetType::Default) {}
};

}

#endif
