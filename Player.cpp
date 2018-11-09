#include "Player.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#define DEG_TO_RADIAN 0.017453293


Player::Player(glm::vec3 eye, glm::vec3 at, glm::vec3 up, GLfloat r) : eye(eye), at(at), up(up), rotation(r) {

}
void Player::moveForward(GLfloat distance) {
    eye = glm::vec3(eye.x + distance * std::sin(rotation*DEG_TO_RADIAN), eye.y, eye.z - distance * std::cos(rotation*DEG_TO_RADIAN));
}

void Player::moveRight(GLfloat distance) {
    eye = glm::vec3(eye.x + distance * std::cos(rotation*DEG_TO_RADIAN), eye.y, eye.z + distance * std::sin(rotation*DEG_TO_RADIAN));
}

void Player::moveUp(GLfloat distance) {
//    eye = glm::vec3(eye.x + distance * std::cos(rotation*DEG_TO_RADIAN), eye.y + distance, eye.z + distance * std::sin(rotation*DEG_TO_RADIAN));
    eye.y += distance;
}

void Player::lookRight(GLfloat distance) {
    rotation += distance;
}

glm::mat4 Player::getCameraDirection() {
    glm::vec3 direction = glm::vec3(eye.x + 1.0f * std::sin(rotation*DEG_TO_RADIAN), eye.y, eye.z - 1.0f * std::cos(rotation*DEG_TO_RADIAN));
    return glm::lookAt(eye, direction, up);
}