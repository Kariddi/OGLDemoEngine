#ifndef __UBERNGINE_SHADER_H__
#define __UBERNGINE_SHADER_H__
#include <string>
#include <OGL.h>

namespace Uberngine {

class Shader {
  GLuint Program;
  bool Valid;

public:
  Shader(std::string vertex_shader, std::string fragment_shader);
  bool isValid() { return Valid; }
  GLuint getProgram() { return Program; }
};

}
#endif
