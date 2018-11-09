// gouraud-tex.vert
// Calculates and passes on V, L, N vectors for use in fragment shader, phong2.frag

// Code partly sourced from the labs and the OpenGL 4.0 Shading Language Cookbook
#version 330

struct lightStruct {
    vec4 ambient; // Ambient light intensity
    vec4 diffuse; // Diffuse light intensity
    vec4 specular; // Specular light intensity
};

struct materialStruct {
    vec4 ambient; // Ambient reflectivity
    vec4 diffuse; // Diffuse reflectivity
    vec4 specular; // Specular reflectivity
    float shininess; // Specular shininess factor
};

uniform mat4 modelview;
uniform mat4 projection;
uniform vec4 lightPosition;
//uniform mat3 normalmatrix;
uniform lightStruct light;
uniform materialStruct material;

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in vec2 in_TexCoord;

out vec3 frontColor;
out vec3 backColor;
out vec2 ex_TexCoord;

vec3 phongModel(vec3 normalizedVertexPosition, vec3 normalizedVertexNormal, vec3 normalizedLightSourcePosition) {
    vec3 v = normalize(-normalizedVertexPosition.xyz);
    vec3 r = reflect(-normalizedLightSourcePosition, normalizedVertexNormal);
    vec3 ambient = vec3(light.ambient) * vec3(material.ambient);
    float sDotN = max(dot(normalizedLightSourcePosition, normalizedVertexNormal), 0.0);
    vec3 diffuse = vec3(light.diffuse * material.diffuse * sDotN);
    vec3 spec = vec3(0.0);
    if (sDotN > 0.0) {
        spec = vec3(light.specular * material.specular * pow(max(dot(r, v), 0.0), material.shininess));
    }
    return ambient + diffuse + spec;
}

void main() {
    // vertex into eye coordinates
    vec4 vertexPosition = modelview * vec4(in_Position, 1.0);
    float ex_D = distance(vertexPosition, lightPosition);
    // Find normalizedVertexPosition(V) - in eye coordinates, eye is at (0,0,0)
    vec3 normalizedVertexPosition = normalize(-vertexPosition).xyz;

    // surface normal in eye coordinates
    // taking the rotation part of the modelview matrix to generate the normal matrix
    // (if scaling is included, should use transpose inverse modelview matrix!)
    // this is somewhat wasteful in compute time and should really be part of the cpu program,
    // giving an additional uniform input
    mat3 normalmatrix = transpose(inverse(mat3(modelview)));
    vec3 normalizedVertexNormal = normalize(normalmatrix * in_Normal);

     // normalizedLightSourcePosition (L) - to light source from vertex
    vec3 normalizedLightSourcePosition = normalize(lightPosition.xyz - normalizedVertexPosition.xyz);

    ex_TexCoord = in_TexCoord;

    gl_Position = projection * vertexPosition;

    frontColor = phongModel(normalizedVertexPosition, normalizedVertexNormal, normalizedLightSourcePosition);
    backColor = phongModel(normalizedVertexPosition, -normalizedVertexNormal, normalizedLightSourcePosition);
}
