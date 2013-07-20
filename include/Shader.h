#ifndef __UBERNGINE_SHADER_H__
#define __UBERNGINE_SHADER_H__
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <OGL.h>

namespace Uberngine {

class PureShader {
protected:
  bool Valid;

public:
  PureShader() : Valid(false) {}
  bool isValid() const { return Valid; }
};

template<typename RendererType>
class Shader : public PureShader {
  GLuint Program;

public:
  Shader(std::string vertex_shader, std::string fragment_shader);
  GLuint getProgram() { return Program; }
};

template<typename RendererType>
Shader<RendererType>::Shader(std::string vertex_shader, std::string fragment_shader) : Program(0) {
  // We need a fragment and a vertex shader
  if (vertex_shader.empty() || fragment_shader.empty())
    return;
  
  std::ifstream ShaderFile;
  std::stringstream ss;
  ShaderFile.open(vertex_shader.c_str(), std::ifstream::in);
  if (!ShaderFile.good()) {
    ShaderFile.close();
    return;
  }
  //Read the shader code
  ss << ShaderFile.rdbuf();
  GLchar *ShaderSource = new GLchar[ss.str().size()+1];
  strcpy(ShaderSource, ss.str().c_str());
  ShaderSource[ss.str().size()] = '\0';

  ShaderFile.close();
  // Create the Vertex shader
  
  GLuint shader[2];
  shader[0] = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(shader[0], 1, (const GLchar**) &ShaderSource, NULL);
  delete [] ShaderSource;
  glCompileShader(shader[0]);
  GLint status;
  //Compile error check
  glGetShaderiv(shader[0], GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    GLint infoLogLength;
    glGetShaderiv(shader[0], GL_INFO_LOG_LENGTH, &infoLogLength);
        
    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    glGetShaderInfoLog(shader[0], infoLogLength, NULL, strInfoLog);
        
    std::cerr << "Compile failure in vertex shader:\n" << strInfoLog << std::endl;
    delete[] strInfoLog;
    return;
  }

  ShaderFile.close();
  ShaderFile.open(fragment_shader.c_str(), std::ifstream::in);
  if (!ShaderFile.good()) {
    return;
  }
  ss.str(std::string());
  //Read the shader source
  ss << ShaderFile.rdbuf();
  ShaderSource = new GLchar[ss.str().size()+1];
  strcpy(ShaderSource, ss.str().c_str());
  ShaderSource[ss.str().size()] = '\0';
  ShaderFile.close();
  shader[1] = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(shader[1], 1, (const GLchar**) &ShaderSource, NULL);
  delete [] ShaderSource;
  glCompileShader(shader[1]);
  //Compile error check
  glGetShaderiv(shader[1], GL_COMPILE_STATUS, &status);

  if (status == GL_FALSE) {
    GLint infoLogLength;
    glGetShaderiv(shader[1], GL_INFO_LOG_LENGTH, &infoLogLength);
        
    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    glGetShaderInfoLog(shader[1], infoLogLength, NULL, strInfoLog);
        
    std::cerr << "Compile failure in fragment shader:\n" <<  strInfoLog << std::endl;
    delete[] strInfoLog;

    return;
  }

  GLuint program = glCreateProgram();
  glAttachShader(program, shader[0]);
  glAttachShader(program, shader[1]);
  //Binding attributes numbers to names
  glBindAttribLocation(program, 0, "position");
  glBindAttribLocation(program, 1, "texcoord");
  glBindAttribLocation(program, 2, "normal");
  //Link the program
  glLinkProgram(program);

  //linking errors check
  glGetProgramiv(program, GL_LINK_STATUS, &status);

  if (status == GL_FALSE) {
    GLint infoLogLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        
    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        
    std::cerr << "Linking failure:\n" << strInfoLog << std::endl;
    delete[] strInfoLog;
    return;

  }
  glDetachShader(program, shader[0]);
  glDetachShader(program, shader[1]);


  Program = program;
  Valid = true;

}

}
#endif
