#ifndef __UBERNGINE_TRAITS_H__
#define __UBERNGINE_TRAITS_H__
#include <OGL.h>

/*
  Some traits used to try to keep the engine interfaces without OpenGL specific types exposed
*/
namespace Uberngine {

struct Traits {
  typedef GLushort MeshIDXType;
  typedef GLfloat MeshVertexType;
};

template<class VertexT, class IndexT>
struct GLMeshTraits {
};

template<>
struct GLMeshTraits<GLfloat, GLuint> {
  static const GLenum VertexDataTy = GL_FLOAT;
  static const GLenum IndexDataTy = GL_UNSIGNED_INT;
};

template<>
struct GLMeshTraits<GLfloat, GLushort> {
  static const GLenum VertexDataTy = GL_FLOAT;
  static const GLenum IndexDataTy = GL_UNSIGNED_SHORT;
};


}

#endif
