// simple vertex shader to render the shadow map
#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 modelview;

void main()
{
    gl_Position = modelview * vec4(position, 1.0f);
}
