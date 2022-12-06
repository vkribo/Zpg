#pragma once

#include "DrawableObject.h"

class DrawableObject;

class Animation {
    bool first = true;
protected:
    virtual void init(DrawableObject& object);
public:
    virtual void step(DrawableObject& object);
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

class RotateAroundObject : public Animation {
    DrawableObject& other;
    float radius;
    float currentAngle;

    glm::mat3 negativePoint = glm::mat3(1);
    glm::mat3 rotationMatrix = glm::mat3(1);
    glm::mat3 positivePoint = glm::mat3(1);
protected:
    void init(DrawableObject &object) override;
public:
    explicit RotateAroundObject(DrawableObject& other);
    void step(DrawableObject& object) override;
};