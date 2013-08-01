varying vec2 frag_texcoord;

uniform sampler2D texture;
uniform vec3 color;

void main() {

  gl_FragColor = vec4(color*texture2D(texture, frag_texcoord).x,1.0);
}
