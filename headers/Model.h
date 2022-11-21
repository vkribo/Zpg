#pragma once

#include <glad.h>
#include <vector>
#include <string>
#include "Texture.h"

class Model {
    unsigned int VAO = -1;
    unsigned int VBO = -1;
    unsigned int IBO = -1;
    GLsizei verticesCount = 0;
    GLsizei indicesCount = 0;
public:
    Model();
    explicit Model(std::vector<float> vertices);
    explicit Model(const float* vertices, size_t size);
    ~Model();
    void draw() const;
    void load(const std::string& fileName);

};