#pragma once

#include <glm/vec3.hpp>

struct Light {
    glm::vec3 position = glm::vec3(0.0);
    glm::vec3 direction = glm::vec3(0.0);
    glm::vec3 ambient = glm::vec3(0.0);
    glm::vec3 diffuse = glm::vec3(0.0);
    glm::vec3 specular = glm::vec3(0.0);
    float cutOff = 0.97629600712;
    float outerCutOff = 0;
    float constant = 1;
    float linear = 0.35;
    float quadratic = 0.44;
};