#ifndef __UBERNGINE_SHADER_H__
#define __UBERNGINE_SHADER_H__
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <OGL.h>
#include <Defs.h>

namespace Uberngine {

class PureShader {
protected:
  bool Valid;

public:
  PureShader() : Valid(false) {}
  bool isValid() const { return Valid; }
};

template<typename RendererType>
class Shader : public PureShader {};

template<>
class Shader<RendererTypes::OpenGL> : public PureShader {
  GLuint Program;

public:
  Shader(std::string vertex_shader, std::string fragment_shader);
  GLuint getProgram() const { return Program; }
};

}
#endif
