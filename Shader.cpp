#include "Shader.h"

#include <string>

#include "rt3d.h"

Shader::Shader() {

}

GLuint Shader::init(const std::string & vertexShaderPath,
                    const std::string & fragmentShaderPath) {
    rt3d::initShaders(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

    return 0;
}
