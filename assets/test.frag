#version 120 

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


uniform mat4 V;

uniform bool is_lighted;
uniform Light L[4];
uniform Material Mat;
uniform int LightNum;

//Textures
uniform sampler2D Textures[1];

varying vec4 Vnormal;
varying vec4 Vpos;
varying vec2 frag_texcoord;
vec4 Vnorm;


vec3 Directional(in int lightnum) {
  vec3 result;
  vec4 Vlightvect = normalize(vec4(L[lightnum].dir, 0.0));
  float diff = max(0.0,dot(Vlightvect, Vnorm));
  vec4 light_dir = reflect(Vlightvect, Vnorm);
  float spec = 0.0;
  if (Mat.spec_c > 0.01)
    spec = pow(max(0.0,dot(light_dir, normalize(-Vpos))), Mat.spec_c);
  vec3 diffuse = texture2D(Textures[0], frag_texcoord).rgb;

  result = Mat.ambient + diffuse*diff + Mat.specular*spec;
  //return Vnorm.xyz;
  return L[lightnum].color*result;
}

void main()
{
   vec3 color = vec3(0.0);
   if (is_lighted) {
     Vnorm = normalize(Vnormal);
     for (int i = 0; i < LightNum; ++i) {
       color += Directional(i);
     }
     gl_FragColor = vec4(color, 1.0);
   } else
     gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
