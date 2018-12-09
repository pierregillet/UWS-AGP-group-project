#version 330 core

out vec4 FragColor;

in vec4 ShadowCoord;

uniform sampler2D shadowMap;

void main()
{
	float visibility = 1.0;
	if ( texture( shadowMap, ShadowCoord.xy ).r  <  ShadowCoord.z - 0.005)
		visibility = 0.5;

	FragColor = vec4(visibility, visibility, visibility, 1.0);
}
