#include "DrawableObject.h"
#include "Transformation.h"

DrawableObject::DrawableObject(const Model &model, std::string shaderName) : model(model), position(0.0), shaderName(std::move(shaderName)) {
    auto t = std::make_unique<Translation>();
    attach(t.get());
    modelMatrix.children.emplace_back(std::move(t));
}

void DrawableObject::draw_object() {
    if (shader == nullptr) {
        return;
    }

    //shader->use();
    shader->update(UpdateValueInfo<glm::mat4>(EventType::SET_SHADER_MODEL, modelMatrix.get()));
    model.draw();
}

void DrawableObject::set_position(glm::vec3 pos) {
    position = pos;
    notify(UpdateValueInfo<glm::vec3>(EventType::OBJ_POS_CHANGED, pos));
}
