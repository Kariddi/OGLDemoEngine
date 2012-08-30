#version 120 

attribute vec3 position;
//in vec3 position;
attribute vec2 texcoord;
attribute vec3 normal;

struct Light {
  vec3 position;
  vec3 dir;
  vec3 color;
  int type;
};

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float spec_c;
};

//Normal View Matrix;
uniform mat4 V;
uniform mat4 MVN;
uniform mat4 P;
uniform mat4 M;
uniform Light L[4];
uniform Material Mat;
uniform int LightNum;

varying vec4 Vnormal;
varying vec4 Vpos;
varying vec2 frag_texcoord;

void main()
{
    Vpos = V*M*vec4(position, 1.0);
    Vnormal = MVN*vec4(normal, 0.0);
    Vnormal.w = 0.0;


    frag_texcoord = texcoord ;
    gl_Position = P*Vpos;
}


