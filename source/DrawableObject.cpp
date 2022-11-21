#include "DrawableObject.h"
#include "Transformation.h"

DrawableObject::DrawableObject(const Model &model, std::string shaderName) : model(model), position(0.0), shaderName(std::move(shaderName)), modelMatrix(std::make_unique<CompositeTransformation>()) {
    attach(modelMatrix.get());
    modelMatrix->children.push_back(std::make_unique<Translation>());
    modelMatrix->children.push_back(std::make_unique<Rotation>());
    modelMatrix->children.push_back(std::make_unique<Scale>());
}

void DrawableObject::draw_object() {
    if (shader == nullptr) {
        return;
    }

    //shader->use();
    shader->update(UpdateValueInfo<glm::mat4>(EventType::SET_SHADER_MODEL, modelMatrix->get()));
    texture.bind();
    shader->update(UpdateValueInfo<unsigned int>(EventType::SET_SHADER_TEXTURE_UNIT, texture.get_texture_id()));
    model.draw();
}

void DrawableObject::set_position(glm::vec3 pos) {
    position = pos;
    notify(UpdateValueInfo<glm::vec3>(EventType::OBJ_POS_CHANGED, pos));
}

void DrawableObject::set_scale(float scale) {
    this->scale = scale;
    notify(UpdateValueInfo<float>(EventType::OBJ_SCALE_CHANGED, scale));
}

void DrawableObject::set_rotation(glm::vec3 rotation) {
    this->rotation = rotation;
    notify(UpdateValueInfo<glm::vec3>(EventType::OBJ_ROTATION_CHANGED, rotation));
}

void DrawableObject::set_texture(Texture tex) {
    texture = tex;
}