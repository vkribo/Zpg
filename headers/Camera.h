#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include "Observer.h"

class Camera : public Subject {
    glm::mat4 view = glm::mat4(1.0);
    glm::vec3 pos = glm::vec3(0.0f,1.0f, 5.0f);
    glm::vec3 target = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
public:
    void update_view();
    void set_pos(glm::vec3 p);
    void set_target(glm::vec3 t);
    void set_up(glm::vec3 u);
    [[nodiscard]] glm::vec3 get_pos() const;
    [[nodiscard]] glm::vec3 get_target() const;
    [[nodiscard]] glm::vec3 get_up() const;
};