#version 330 core
layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 InputTexCoord;

out vec2 TexCoord;

void main() {
    TexCoord = InputTexCoord;
    gl_Position = vec4(Position, 1.0);
}