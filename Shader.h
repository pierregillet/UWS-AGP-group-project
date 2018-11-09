#ifndef AGP_GROUP_PROJECT_SHADER_H
#define AGP_GROUP_PROJECT_SHADER_H

#include <GL/glew.h>
#include <string>


class Shader {
public:
    static GLuint init(const std::string & vertexShaderPath,
                       const std::string & fragmentShaderPath);

private:
    Shader();
};


#endif //AGP_GROUP_PROJECT_SHADER_H
