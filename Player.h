#ifndef AGP_INDIVIDUAL_PROJECT_PLAYER_H
#define AGP_INDIVIDUAL_PROJECT_PLAYER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

class Player {
private:
    glm::vec3 eye;
    glm::vec3 at;
    glm::vec3 up;
    GLfloat rotation;

public:
    Player(glm::vec3 eye, glm::vec3 at, glm::vec3 up, GLfloat r);
    void moveForward(GLfloat distance);
    void moveRight(GLfloat distance);
    void moveUp(GLfloat distance);
    void lookRight(GLfloat distance);
    glm::mat4 getCameraDirection();
};

#endif //AGP_INDIVIDUAL_PROJECT_PLAYER_H
