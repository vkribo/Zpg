#pragma once

#include "DrawableObject.h"

class DrawableObject;

class Animation {
public:
    virtual void step(DrawableObject& object) = 0;
    virtual ~Animation() = default;
};

class RepeatedLineMovement : public Animation {
    glm::vec3 lineVector;
    float reverseDistance;
    float stepDistance;
    float currentDistance;
public:
    RepeatedLineMovement(glm::vec3 lineVector, float reverseDistance);
    void step(DrawableObject& object) override;
};