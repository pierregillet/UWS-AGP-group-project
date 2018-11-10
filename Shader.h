#ifndef AGP_GROUP_PROJECT_SHADER_H
#define AGP_GROUP_PROJECT_SHADER_H

#include <GL/glew.h>
#include <string>

#include "rt3d.h"


class Shader {
public:
    static GLuint init(const std::string & vertexShaderPath,
                       const std::string & fragmentShaderPath);

    static void setLight(const GLuint & shader, const rt3d::lightStruct & light, const rt3d::materialStruct & material);

private:
    Shader();
};


#endif //AGP_GROUP_PROJECT_SHADER_H
