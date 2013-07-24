#ifndef __UBERNGINE_GLTEXTURE_H__
#define __UBERNGINE_GLTEXTURE_H__

#include <Renderers/Texture.h>
#include <Defs.h>

namespace Uberngine {

template<>
class Texture<RendererTypes::OpenGL> : public PureTexture {

  GLuint GLTexture;

public:
  Texture(GLuint gl_texture) : GLTexture(gl_texture);

  GLuint GetGLTexture() const { return GLTexture; } 

};

}

#endif
