#pragma once

#include "Shader.h"
#include "Model.h"
#include "Transformation.h"
#include "Observer.h"

class DrawableObject : public Subject {
    const Model& model;
    std::unique_ptr<CompositeTransformation> modelMatrix;
    glm::vec3 position;
    float scale;
    glm::vec3 rotation;
public:
    Shader* shader = nullptr;
    std::string shaderName;
    explicit DrawableObject(const Model &model, std::string shaderName);
    void draw_object();
    void set_position(glm::vec3 pos);
    void set_scale(float scale);
    void set_rotation(glm::vec3 rotation);
};