// textured.frag
#version 330

// Some drivers require the following
precision highp float;

uniform samplerCube textureUnit1;
uniform sampler2D textureUnit0;

in vec2 ex_TexCoord;
in vec3 ex_WorldNorm;
in vec3 ex_WorldView;

layout(location = 0) out vec4 out_Color;
 
void main(void) {
	vec3 reflectTexCoord = reflect(-ex_WorldView, normalize(ex_WorldNorm));
	out_Color =	texture(cubeMap, reflectTexCoord) * texture(texMap, ex_TexCoord) * litColor;

	// Fragment	colour
    out_Color = texture(textureUnit0, ex_TexCoord);
}
