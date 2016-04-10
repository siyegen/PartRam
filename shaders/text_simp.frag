#version 330 core
in vec2 TexCoord;
out vec4 color;

uniform sampler2D ourTexture;
uniform vec3 textColor;

void main()
{
    color = vec4(textColor, 1.0f) * texture(ourTexture, TexCoord);
}