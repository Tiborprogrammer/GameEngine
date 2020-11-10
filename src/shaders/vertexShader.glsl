#version 330
layout (location=0) in vec3 vp;

uniform mat4 cameraMatrix;
out vec4 position;

void main() {
  gl_Position = cameraMatrix * vec4(vp, 1.0);
  position = gl_Position;
}