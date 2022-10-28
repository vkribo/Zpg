#pragma once

#include <glad.h>
#include <vector>

class Model {
    unsigned int vao = -1;
    unsigned int vbo = -1;
    GLsizei verticesCount;
public:
    explicit Model(std::vector<float> vertices);
    ~Model();
    void draw() const;
};