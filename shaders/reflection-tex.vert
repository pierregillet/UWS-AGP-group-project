// textured.vert
// use textures, but no lighting
#version 330

uniform mat4 modelview;
uniform mat4 projection;
uniform mat4 modelMatrix;
uniform vec3 cameraPos;


in  vec3 in_Position;
//in  vec3 in_Colour; // colour not used with lighting
//in  vec3 in_Normal;

in vec2 in_TexCoord;
out vec2 ex_TexCoord;
out vec3 ex_WorldNorm;
out vec3 ex_WorldView;
//out vec4 ex_Color;

// multiply each vertex position by the MVP matrix
void main(void) {
    ex_TexCoord = in_TexCoord;
	vec3 worldPos = (modelMatrix * vec4(in_Position, 1.0)).xyz;
	mat3 normalworldmatrix = transpose(inverse(mat3(modelMatrix)));
	// This	is expensive, better to	compute	it on the CPU and set it as
	// an uniform as suggested in lab 2
	ex_WorldNorm = normalworldmatrix * in_Normal;
	ex_WorldView = cameraPos - worldPos;
	ex_TexCoord	= in_TexCoord;

    // vertex into eye coordinates
    vec4 vertexPosition = modelview * vec4(in_Position,1.0);
    gl_Position = projection * vertexPosition;
}
