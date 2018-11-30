#version 330

in vec2 in_position;
in vec2 in_textureCoordinates;

out vec2 out_textureCoordinates;

void main()
{
    out_textureCoordinates = in_textureCoordinates;
    gl_Position = vec4(in_position, 0.0, 1.0);
}
