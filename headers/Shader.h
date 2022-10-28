#pragma once

#include <string>
#include <glad.h>
#include <vector>



enum class ShaderType {
    VERTEX,
    FRAGMENT
};

class Shader {
    std::vector<std::pair<std::string, ShaderType>> shaders;
    unsigned int shaderProgram = -1;

    static unsigned int create_shader(const std::string& source, ShaderType type);
    void create_shaders();
public:
    explicit Shader(std::vector<std::pair<std::string, ShaderType>> shaders);
    unsigned int get_program_id() const;
    ~Shader();
    void use() const;
};

