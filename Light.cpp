#include "Light.h"

Light::Light(const glm::vec4 & position) : position(position) {

}

void Light::moveForward(const GLfloat & distance) {
    this->position[2] += distance;
}

void Light::moveRight(const GLfloat & distance) {
    this->position[0] += distance;
}

void Light::moveUp(const GLfloat & distance) {
    this->position[1] += distance;
}

glm::vec4 Light::getPosition() {
    return this->position;
}

void Light::setPosition(const glm::vec4 & position) {
    this->position = position;
}
