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
    this->front = front;
    this->up = up;
    this->left = glm::cross(up, front);
}

glm::mat4 Camera::view() {
    return glm::lookAt(this->pos, this->pos + this->front, this->up);
}
