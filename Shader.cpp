#include "Shader.h"

#include <string>

#include "constants.h"
#include "rt3d.h"

Shader::Shader() = default;

GLuint Shader::init(const std::string & vertexShaderPath,
                    const std::string & fragmentShaderPath) {
    rt3d::initShaders(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

    return 0;
}

void Shader::setLight(const GLuint & shader, const rt3d::lightStruct & light, const rt3d::materialStruct & material) {
    rt3d::setLight(shader, light);
    rt3d::setMaterial(shader, material);
    // set light attenuation shader uniforms
    glUniform1f(glGetUniformLocation(shader, "attConst"), Constants::LightAttenuation::constant);
    glUniform1f(glGetUniformLocation(shader, "attLinear"), Constants::LightAttenuation::linear);
    glUniform1f(glGetUniformLocation(shader, "attQuadratic"), Constants::LightAttenuation::quadratic);
}


