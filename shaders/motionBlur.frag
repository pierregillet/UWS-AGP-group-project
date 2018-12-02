#version 330

#define MAX_BUFFERED_TEXTURES 5

uniform sampler2D[MAX_BUFFERED_TEXTURES] bufferedTextures;

in vec2 out_textureCoordinates;

out vec4 out_fragmentColor;

void main()
{
    
	out_fragmentColor = (
		texture(bufferedTextures[0], out_textureCoordinates)
		+ texture(bufferedTextures[1], out_textureCoordinates)
		+ texture(bufferedTextures[2], out_textureCoordinates)
		+ texture(bufferedTextures[3], out_textureCoordinates)
		+ texture(bufferedTextures[4], out_textureCoordinates)
	) / 5.0f;
}
