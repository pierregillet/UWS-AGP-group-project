#include "Player.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "constants.h"

Player::Player(glm::vec3 eye, glm::vec3 at, glm::vec3 up, GLfloat r)
        : eye(eye), at(at), up(up), rotation(r) {

}
void Player::moveForward(GLfloat distance) {
    eye = glm::vec3(eye.x + distance * std::sin(rotation * Constants::degreeToRadian),
                    eye.y, eye.z - distance * std::cos(rotation * Constants::degreeToRadian));
}

void Player::moveRight(GLfloat distance) {
    eye = glm::vec3(eye.x + distance * std::cos(rotation * Constants::degreeToRadian),
                    eye.y, eye.z + distance * std::sin(rotation * Constants::degreeToRadian));
}

void Player::moveUp(GLfloat distance) {
    eye.y += distance;
}

void Player::lookRight(GLfloat distance) {
    rotation += distance;
}

glm::mat4 Player::getCameraDirection() {
    glm::vec3 direction = glm::vec3(eye.x + 1.0f * std::sin(rotation * Constants::degreeToRadian), eye.y, eye.z - 1.0f * std::cos(rotation * Constants::degreeToRadian));
    return glm::lookAt(eye, direction, up);
}