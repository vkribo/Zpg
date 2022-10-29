#pragma once

#include <string>
#include <glad.h>
#include <vector>
#include <glm/ext/matrix_float4x4.hpp>
#include "Observer.h"


enum class ShaderType {
    VERTEX,
    FRAGMENT
};

class Shader : public Observer {
    std::vector<std::pair<std::string, ShaderType>> shaders;
    unsigned int shaderProgram = -1;
    bool inUse = false;

    static unsigned int create_shader(const std::string& source, ShaderType type);
    void create_shaders();
    void send_matrix(const char* varName, const glm::mat4 &matrix);
    void send_vec(const char* varName, const glm::vec3 &vec);
public:
    explicit Shader(std::vector<std::pair<std::string, ShaderType>> shaders);
    unsigned int get_program_id() const;
    ~Shader();
    void use();
    void unuse();
    void update(const UpdateInfo& info) override;
};

