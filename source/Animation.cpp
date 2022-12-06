#include "Animation.h"

RepeatedLineMovement::RepeatedLineMovement(glm::vec3 lineVector, float reverseDistance)
    : lineVector(glm::normalize(lineVector)), reverseDistance(reverseDistance)
{
    stepDistance = 0.1;
}

void RepeatedLineMovement::step(DrawableObject &object) {
    auto pos = object.get_position();
    pos += lineVector * stepDistance;
    object.set_position(pos);
    if (currentDistance > reverseDistance) {
        lineVector *= -1;
        currentDistance = 0;
    }

    currentDistance += stepDistance;
}


RotateAroundObject::RotateAroundObject(DrawableObject& other)
    : other(other)
{
    float angle = 1 * (M_PI / 180);
    rotationMatrix[0][0] = cosf(angle);
    rotationMatrix[1][1] = cosf(angle);
    rotationMatrix[1][0] = -sinf(angle);
    rotationMatrix[0][1] = sinf(angle);
}

void RotateAroundObject::step(DrawableObject &object) {
    auto other_pos = other.get_position();
    auto neg = -other_pos;
    negativePoint[2] = glm::vec3(neg.x, neg.z, 1.0);
    positivePoint[2] = glm::vec3(other_pos.x, other_pos.z, 1.0);
    auto res = positivePoint * rotationMatrix * negativePoint;
    auto obj_pos = object.get_position();
    auto vec = res * glm::vec3(obj_pos.x, obj_pos.z, 1);
    obj_pos.x = vec.x;
    obj_pos.z = vec.y;
    object.set_position(obj_pos);
}

void RotateAroundObject::init(DrawableObject &object) {
    auto other_pos = other.get_position();
    object.set_position(glm::vec3(other_pos.x + radius, other_pos.y, other_pos.z));
}

void Animation::step(DrawableObject &object) {
    if (first) {
        init(object);
        first = false;
    }
}

void Animation::init(DrawableObject &object) {
}
