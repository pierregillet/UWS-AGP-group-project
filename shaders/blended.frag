#version 330

precision highp float;

uniform sampler2D baseTex;
uniform sampler2D effectTex;

in vec2 ex_TexCoord;

layout(location = 0) out vec4 fragmentColor;
 
void main(void) {
    vec4 brickTexColor = texture(baseTex, ex_TexCoord);
    vec4 mossTexColor = texture(effectTex, ex_TexCoord);
    vec4 color = mix(brickTexColor, mossTexColor, mossTexColor.a);
    // Adding the alpha value to 1.0 to force the object to be opaque
    fragmentColor = vec4(color.rgb, 1.0);
}
