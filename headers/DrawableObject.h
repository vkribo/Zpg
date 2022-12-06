#pragma once

#include "Shader.h"
#include "Model.h"
#include "Transformation.h"
#include "Observer.h"
#include "Animation.h"

class Animation;

class DrawableObject : public Subject {
    const Model& model;
    std::unique_ptr<CompositeTransformation> modelMatrix;
    glm::vec3 position;
    float scale;
    glm::vec3 rotation;
    Texture texture;
    std::vector<std::unique_ptr<Animation>> animations;
public:
    std::string shaderName;
    explicit DrawableObject(const Model &model, std::string shaderName);
    void draw_object(Shader *shader);
    glm::vec3 get_position();
    void set_position(glm::vec3 pos);
    void set_scale(float scale);
    void set_rotation(glm::vec3 rotation);
    void set_texture(Texture tex);
    void step_animation();
    void add_animation(std::unique_ptr<Animation>&& animation);
};