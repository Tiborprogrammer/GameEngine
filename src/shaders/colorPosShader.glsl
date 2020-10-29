#version 330
layout (location=0) out vec4 frag_colour;
in vec4 position;

void main() {
    frag_colour = position;
}