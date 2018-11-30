#version 330

//#define MAX_BUFFERED_TEXTURES 5

//uniform sampler2D[MAX_BUFFERED_TEXTURES] bufferedTextures;

uniform sampler2D in_texture;

in vec2 in_textureCoordinates;

out vec4 out_fragmentColor;

void main()
{
//    vec4 tmpColor = mix(texture(bufferedTextures[0], in_textureCoordinates),
//                        texture(bufferedTextures[1], in_textureCoordinates),
//                        0.75);
//
//    tmpColor = mix(tmpColor, texture(bufferedTextures[2], in_textureCoordinates), 0.75);
//
//    tmpColor = mix(tmpColor, texture(bufferedTextures[3], in_textureCoordinates), 0.75);
//    tmpColor = mix(tmpColor, texture(bufferedTextures[4], in_textureCoordinates), 0.75);
//    tmpColor.a = 1.0;
//
//    out_fragmentColor = tmpColor;

    out_fragmentColor = texture(in_texture, in_textureCoordinates);
}