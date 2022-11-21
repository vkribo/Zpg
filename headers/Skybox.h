#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include "Model.h"
#include "Shader.h"

class Skybox {
    Model cube;
    Texture texture;
public:
    Shader* skybox_shader;
    Skybox() = default;
    explicit Skybox(Shader *skybox_shader);
    void draw();
};