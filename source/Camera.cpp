#include <glm/ext/matrix_transform.hpp>
#include "Camera.h"

void Camera::set_pos(glm::vec3 p) {
    pos = p;
    update_view();
}

void Camera::set_target(glm::vec3 t) {
    target = t;
    update_view();
}

void Camera::set_up(glm::vec3 u) {
    up = u;
    update_view();
}

glm::vec3 Camera::get_pos() const {
    return pos;
}

glm::vec3 Camera::get_target() const {
    return target;
}

glm::vec3 Camera::get_up() const {
    return up;
}

void Camera::update_view() {
    view = glm::lookAt(pos, pos + target, up);
    notify(UpdateValueInfo<glm::mat4>(EventType::SET_SHADER_VIEW, view));
    notify(UpdateValueInfo<glm::vec3>(EventType::SET_SHADER_VIEWPOS, pos));
}
