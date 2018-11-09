// Gouraud fragment shader gouraud-tex.frag matched with gouraud-tex.vert

// Code partly sourced from the labs and the OpenGL 4.0 Shading Language Cookbook

#version 330

// Some drivers require the following
precision highp float;

uniform sampler2D textureUnit0;

in vec3 frontColor;
in vec3 backColor;
in vec2 ex_TexCoord;

layout( location = 0 ) out vec4 FragColor;

void main() {
    if (gl_FrontFacing) {
        FragColor = vec4(frontColor, 1.0) * texture(textureUnit0, ex_TexCoord); //min(vec4(frontColor, 1.0) * texture(textureUnit0, ex_TexCoord),vec4(1.0f));//, 1.0);// + texture(textureUnit0, ex_TexCoord), 1.0);
    } else {
        FragColor = vec4(backColor, 1.0) * texture(textureUnit0, ex_TexCoord); //min(vec4(backColor, 1.0) * texture(textureUnit0, ex_TexCoord),vec4(1.0f));//vec4(backColor, 1.0);// + texture(textureUnit0, ex_TexCoord), 1.0);
    }

}
