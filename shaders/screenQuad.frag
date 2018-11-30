#version 330

uniform sampler2D framebufferTexture;

in vec2 textureCoordinates;

out vec4 fragmentColor;


void main()
{
    fragmentColor = texture(framebufferTexture, textureCoordinates);
}
