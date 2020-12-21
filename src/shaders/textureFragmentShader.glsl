#version 330 core

layout (location=0) out vec4 frag_colour;

in vec2 TexCoord;

uniform sampler2D TextureImage;

void main()
{
    frag_colour = texture(TextureImage, TexCoord);
}