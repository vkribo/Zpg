#pragma once

#include <glm/mat4x4.hpp>
#include <vector>
#include <memory>
#include "Observer.h"

class Transformation : public Observer {
public:
    [[nodiscard]] virtual glm::mat4 get() const = 0;
    virtual ~Transformation() = default;
};

class CompositeTransformation : public Transformation {
public:
    std::vector<std::unique_ptr<Transformation>> children;

    [[nodiscard]] glm::mat4 get() const override;

    void update(const UpdateInfo& info) override;
};

class Translation : public Transformation {
    glm::mat4 translationMatrix = glm::mat4(1.0);
public:
    void update(const UpdateInfo &info) override;
    [[nodiscard]] glm::mat4 get() const override;
};

class Scale : public Transformation {
    glm::mat4 scaleMatrix = glm::mat4(1.0);
public:
    void update(const UpdateInfo& info) override;
    [[nodiscard]] glm::mat4 get() const override;
};

class RotationComponent : public Transformation {
protected:
    glm::mat4 rotationMatrix = glm::mat4(1.0);
public:
    [[nodiscard]] glm::mat4 get() const override;
};

class RotationX : public RotationComponent {
public:
    void update(const UpdateInfo& info) override;
};

class RotationY : public RotationComponent {
public:
    void update(const UpdateInfo& info) override;
};

class RotationZ : public RotationComponent {
public:
    void update(const UpdateInfo& info) override;
};

class Rotation : public CompositeTransformation {
public:
    Rotation();
};