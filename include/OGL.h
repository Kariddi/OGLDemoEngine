#ifndef __UBERNGINE_OGL_H__
#define __UBERNGINE_OGL_H__

#if defined(__APPLE__)
#include <TargetConditionals.h>
#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#define OGL_ES
#else
#include <OpenGL/gl.h>
#define OGL
#endif
#if defined(OGL)
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#elif defined(OGL_ES)
#define glGenVertexArrays glGenVertexArraysOES
#define glBindVertexArray glBindVertexArrayOES
#define glDeleteVertexArrays glDeleteVertexArraysOES
#define glClearDepth glClearDepthf
#else
#include <GL/glew.h>
#endif
#endif
#endif
