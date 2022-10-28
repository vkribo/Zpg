#include "Transformation.h"

glm::mat4 CompositeTransformation::get() const {
    glm::mat4 result(1.0);
    for (const auto& t : children) {
        result *= t->get();
    }

    return result;
}

Translation::Translation() : translationMatrix(1.0) {
}

glm::mat4 Translation::get() const {
    return translationMatrix;
}

void Translation::update(const UpdateInfo &info) {
    if (info.type == EventType::OBJ_POS_CHANGED) {
        translationMatrix[3] = glm::vec4(dynamic_cast<const UpdateValueInfo<glm::vec3>&>(info).value, 1.0);
    }
}
