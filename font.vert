#version 120

attribute vec2 position;
attribute vec2 texcoord;
uniform float width;
uniform float height;

varying vec2 frag_texcoord;

void main() {
  frag_texcoord = texcoord;
  vec2 pos = vec2(2*position.x/width, 2*position.y/height) - 1.0;
  gl_Position = vec4(pos, 0.0, 1.0);
}
