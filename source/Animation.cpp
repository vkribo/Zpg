#include "Animation.h"

RepeatedLineMovement::RepeatedLineMovement(glm::vec3 lineVector, float reverseDistance)
    : lineVector(glm::normalize(lineVector)), reverseDistance(reverseDistance)
{
    stepDistance = 0.2;
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
