#include "Shader.h"

#include <iostream>
#include <glad.h>
#include <glm/ext/matrix_float4x4.hpp>

void Shader::create_shaders() {
    shaderProgram = glCreateProgram();
    std::vector<unsigned int> toDelete;
    for (const auto& s : shaders) {
        auto x = create_shader(s.first, s.second);
        toDelete.push_back(x);
        glAttachShader(shaderProgram, x);
    }

    glLinkProgram(shaderProgram);
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        throw std::runtime_error("Shader compilation failed");
    }

    for (auto x : toDelete) {
        glDeleteShader(x);
    }
}

Shader::Shader(std::vector<std::pair<std::string, ShaderType>> shaders) : shaders(std::move(shaders))
{
    create_shaders();
}

unsigned int Shader::get_program_id() const {
    return shaderProgram;
}

Shader::~Shader() {
    glDeleteProgram(shaderProgram);
}

void Shader::use() {
    inUse = true;
    glUseProgram(shaderProgram);
}

unsigned int Shader::create_shader(const std::string &source, ShaderType type) {
    auto shd = source.c_str();
    unsigned int shader;
    switch (type) {
        case ShaderType::FRAGMENT:
            shader = glCreateShader(GL_FRAGMENT_SHADER);
        break;

        case ShaderType::VERTEX:
            shader = glCreateShader(GL_VERTEX_SHADER);
        break;
        default:
            throw std::runtime_error("Not implemented");
    }

    glShaderSource(shader, 1, &shd, NULL);
    glCompileShader(shader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

void Shader::update(const UpdateInfo &info) {
    if (!inUse) {
        return;
    }

    switch (info.type) {
        case EventType::SET_SHADER_MODEL:
            send_matrix("gModel", dynamic_cast<const UpdateValueInfo<glm::mat4>&>(info).value);
        break;
        case EventType::SET_SHADER_PROJECTION:
            send_matrix("gProjection", dynamic_cast<const UpdateValueInfo<glm::mat4>&>(info).value);
        break;
        case EventType::SET_SHADER_VIEW:
            send_matrix("gView", dynamic_cast<const UpdateValueInfo<glm::mat4>&>(info).value);
        break;
        case EventType::SET_SHADER_VIEWPOS:
            send_vec("viewPos", dynamic_cast<const UpdateValueInfo<glm::vec3>&>(info).value);
        default:
            return;
    }
}

void Shader::send_matrix(const char *varName, const glm::mat4 &matrix) {
    glUniformMatrix4fv(
            glGetUniformLocation(shaderProgram, varName), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::send_vec(const char *varName, const glm::vec3 &vec) {
    glUniform3fv(glGetUniformLocation(shaderProgram, varName), 1, &vec[0]);
}

void Shader::unuse() {
    inUse = false;
}
