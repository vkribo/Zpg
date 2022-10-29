#pragma once

#include "Shader.h"
#include "Model.h"
#include "Transformation.h"
#include "Observer.h"

class DrawableObject : public Subject {
    const Model& model;
    CompositeTransformation modelMatrix;
    glm::vec3 position;
public:
    Shader* shader = nullptr;
    std::string shaderName;
    explicit DrawableObject(const Model &model, std::string shaderName);
    void draw_object();
    void set_position(glm::vec3 pos);
};