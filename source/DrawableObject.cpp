#include "DrawableObject.h"
#include "Transformation.h"

DrawableObject::DrawableObject(const Model &model) : model(model), position(0.0), shader(nullptr) {
    auto t = std::make_unique<Translation>();
    attach(t.get());
    modelMatrix.children.emplace_back(std::move(t));
}

void DrawableObject::draw_object() {
    if (shader == nullptr) {
        return;
    }
    shader->use();
    glUniformMatrix4fv(glGetUniformLocation(shader->get_program_id(), "gModel"), 1, GL_FALSE, &modelMatrix.get()[0][0]);
    model.draw();
}

void DrawableObject::set_shader(Shader *s) {
    shader = s;
}

void DrawableObject::update(const UpdateInfo &info) {
    switch (info.type) {
        case EventType::SET_SHADER:
            shader = dynamic_cast<const UpdateValueInfo<Shader*>&>(info).value;
        break;
        case EventType::DRAW:
            draw_object();
        break;
    }
}

void DrawableObject::set_position(glm::vec3 pos) {
    position = pos;
    notify(UpdateValueInfo<glm::vec3>(EventType::OBJ_POS_CHANGED, pos));
}

