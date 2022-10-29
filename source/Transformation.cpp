#include "Transformation.h"

inline float radians(float degrees) {
    constexpr float mult = M_PI / 180;
    return degrees * mult;
}

glm::mat4 CompositeTransformation::get() const {
    glm::mat4 result(1.0);
    for (const auto& t : children) {
        result *= t->get();
    }

    return result;
}

void CompositeTransformation::update(const UpdateInfo &info) {
    for (const auto& t : children) {
        t->update(info);
    }
}

glm::mat4 Translation::get() const {
    return translationMatrix;
}

void Translation::update(const UpdateInfo &info) {
    if (info.type == EventType::OBJ_POS_CHANGED) {
        translationMatrix[3] = glm::vec4(info.get_val<glm::vec3>(), 1.0);
    }
}

void Scale::update(const UpdateInfo &info) {
    if (info.type == EventType::OBJ_SCALE_CHANGED) {
        auto newScale = info.get_val<float>();
        scaleMatrix[0][0] = scaleMatrix[1][1] = scaleMatrix[2][2] = newScale;
    }
}

glm::mat4 Scale::get() const {
    return scaleMatrix;
}

void RotationX::update(const UpdateInfo &info) {
    if (info.type == EventType::OBJ_ROTATION_CHANGED) {
        auto xAngle = radians(info.get_val<glm::vec3>()[0]);
        auto cosX = std::cos(xAngle);
        auto sinX = std::sin(xAngle);
        rotationMatrix[1][1] = rotationMatrix[2][2] = cosX;
        rotationMatrix[2][1] = -sinX;
        rotationMatrix[1][2] = sinX;
    }
}

glm::mat4 RotationComponent::get() const {
    return rotationMatrix;
}

void RotationY::update(const UpdateInfo &info) {
    if (info.type == EventType::OBJ_ROTATION_CHANGED) {
        auto yAngle = radians(info.get_val<glm::vec3>()[1]);
        auto cosY = std::cos(yAngle);
        auto sinY = std::sin(yAngle);
        rotationMatrix[0][0] = rotationMatrix[2][2] = cosY;
        rotationMatrix[2][0] = sinY;
        rotationMatrix[0][2] = -sinY;
    }
}

void RotationZ::update(const UpdateInfo &info) {
    if (info.type == EventType::OBJ_ROTATION_CHANGED) {
        auto zAngle = radians(info.get_val<glm::vec3>()[2]);
        auto cosZ = std::cos(zAngle);
        auto sinZ = std::sin(zAngle);
        rotationMatrix[0][0] = rotationMatrix[1][1] = cosZ;
        rotationMatrix[1][0] = -sinZ;
        rotationMatrix[0][1] = sinZ;
    }
}

Rotation::Rotation() {
    children.push_back(std::make_unique<RotationX>());
    children.push_back(std::make_unique<RotationY>());
    children.push_back(std::make_unique<RotationZ>());
}
