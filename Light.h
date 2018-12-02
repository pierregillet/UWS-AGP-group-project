#ifndef AGP_GROUP_PROJECT_LIGHT_H
#define AGP_GROUP_PROJECT_LIGHT_H

#include <glm/glm.hpp>
#include <GL/glew.h>


class Light {
public:
    explicit Light(const glm::vec4 & position);
    void moveForward(const GLfloat & distance);
    void moveRight(const GLfloat & distance);
    void moveUp(const GLfloat & distance);
    glm::vec4 getPosition();
    void setPosition(const glm::vec4 & position);

private:
    glm::vec4 position;
};


#endif //AGP_GROUP_PROJECT_LIGHT_H
