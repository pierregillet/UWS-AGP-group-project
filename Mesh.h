#ifndef AGP_GROUP_PROJECT_DRAWABLEOBJECT_H
#define AGP_GROUP_PROJECT_DRAWABLEOBJECT_H

#include <GL/glew.h>
#include <string>

class Mesh {
private:
    GLuint meshIndexCount;
    GLuint meshObject;

public:
    explicit Mesh(const std::string & objectPath);
    explicit Mesh(GLuint vao);

    GLuint getMeshIndexCount();
    GLuint getMeshObject();
};


#endif //AGP_GROUP_PROJECT_DRAWABLEOBJECT_H
