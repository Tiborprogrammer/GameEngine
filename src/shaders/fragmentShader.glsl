#version 330
layout (location=0) out vec4 frag_colour;
uniform vec4 color = vec4(0, 0, 0, 1);

void main() {
    frag_colour = color;
}