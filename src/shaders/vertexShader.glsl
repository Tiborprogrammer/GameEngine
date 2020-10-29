#version 330
layout (location=0) in vec3 vp;
out vec4 position;

void main() {
  gl_Position = vec4(vp, 1.0);
  position = vec4(vp, 1.0);
}