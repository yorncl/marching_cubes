#include "vox.h"
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>


Camera::Camera() {
}

Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up) {
    this->pos = pos;
    this->front = glm::normalize(front);
    this->up = glm::normalize(up);
    this->left = glm::cross(up, front);
}

// TODO refactor
glm::mat4 Camera::view() {
    return glm::lookAt(this->pos, this->pos + this->front, this->up);
}


// TODO fix
void camera_rotate(Camera &c, float pitch, float yaw, float roll) {

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    c.front = glm::normalize(direction);
    // c.left = glm::cross(c.front, glm::vec3(0.0, 1.0, 0.0));
    // c.up = glm::cross(c.front, c.left);
}
