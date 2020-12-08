#version 330 core

layout(location = 0) out vec4 color;

in vec2 TexCoord;

uniform sampler2D TextureImage;

void main()
{
    color = texture(TextureImage, TexCoord);
}