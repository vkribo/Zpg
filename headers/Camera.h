#pragma once

#include <glm/vec3.hpp>

struct Camera {
    glm::vec3 pos = glm::vec3(0.0f,0.0f, 5.0f);
    glm::vec3 target = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
};