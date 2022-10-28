#pragma once

#include "Shader.h"
#include "Model.h"
#include "Transformation.h"
#include "Observer.h"

class DrawableObject : public Subject, public Observer {
    const Model& model;

    const Shader* shader;
    CompositeTransformation modelMatrix;
    glm::vec3 position;
public:
    explicit DrawableObject(const Model& model);
    void update(const UpdateInfo &info) override;
    void draw_object();
    void set_shader(Shader* s);
    void set_position(glm::vec3 pos);
};