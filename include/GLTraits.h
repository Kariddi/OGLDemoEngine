#ifndef __UBERNGINE_GLTRAITS_H__
#define __UBERNGINE_GLTRAITS_H__
#include <OGL.h>
#include <Defs.h>

/*
  Some traits used to try to keep the engine interfaces without OpenGL specific types exposed
*/
namespace Uberngine {

template<int type_size>
struct GLTypeTraits {
};

template<>
struct GLTypeTraits<1> {
  static const GLenum GLDataType = GL_UNSIGNED_BYTE;
  static const GLenum GLIndexType = GL_UNSIGNED_BYTE;
};

template<>
struct GLTypeTraits<2> {
  static const GLenum GLDataType = GL_UNSIGNED_SHORT;
  static const GLenum GLIndexType = GL_UNSIGNED_SHORT;
};

template<>
struct GLTypeTraits<4> {
  static const GLenum GLDataType = GL_FLOAT;
  static const GLenum GLIndexType = GL_UNSIGNED_INT;
};

}

#endif
