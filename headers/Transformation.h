#pragma once

#include <glm/mat4x4.hpp>
#include <vector>
#include <memory>
#include "Observer.h"

class Transformation {
public:
    [[nodiscard]] virtual glm::mat4 get() const = 0;
};

class CompositeTransformation : public Transformation {
public:
    std::vector<std::shared_ptr<Transformation>> children;

    [[nodiscard]] glm::mat4 get() const override;

    template<class T>
    T* find(std::vector<std::shared_ptr<Transformation>>* ch = nullptr) {
        for (const auto& c : children) {
            auto casted = dynamic_cast<T*>(c.get());
            if (casted != nullptr) {
                return casted;
            }

            auto comp = dynamic_cast<CompositeTransformation*>(c.get());
            if (comp != nullptr) {
                return find<T>(&comp->children);
            }
        }

        return nullptr;
    }
};

class Translation : public Transformation, public Observer {
    glm::mat4 translationMatrix;
public:
    Translation();
    void update(const UpdateInfo &info) override;
    [[nodiscard]] glm::mat4 get() const override;
};