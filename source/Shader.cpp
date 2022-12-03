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
    if (inUse) {
        return;
    }

    inUse = true;
    glUseProgram(shaderProgram);
    if (model)
        send_matrix("gModel", *model);
    if (projection)
        send_matrix("gProjection", *projection);
    if (view)
        send_matrix("gView", *view);
    if (viewPos)
        send_vec("viewPos", *viewPos);
    if (dir_lights)
        send_lights("dir_lights", *dir_lights);
    if (point_lights)
        send_lights("point_lights", *point_lights);
    if (spot_lights)
        send_lights("spot_lights", *spot_lights);
    if (flashlight)
        send_flashlight(*flashlight);
    glUniform1ui(glGetUniformLocation(shaderProgram, "textureUnitID"), textureUnit);
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
    switch (info.type) {
        case EventType::SET_SHADER_MODEL:
            model = &dynamic_cast<const UpdateValueInfo<glm::mat4>&>(info).value;
        break;
        case EventType::SET_SHADER_PROJECTION:
            projection = &dynamic_cast<const UpdateValueInfo<glm::mat4>&>(info).value;
        break;
        case EventType::SET_SHADER_VIEW:
            view = &dynamic_cast<const UpdateValueInfo<glm::mat4>&>(info).value;
        break;
        case EventType::SET_SHADER_VIEWPOS:
            viewPos = &dynamic_cast<const UpdateValueInfo<glm::vec3>&>(info).value;
        break;
        case EventType::SET_SHADER_DIR_LIGHTS:
            dir_lights = &info.get_val<std::vector<Light>>();
        case EventType::SET_SHADER_POINT_LIGHTS:
            point_lights = &info.get_val<std::vector<Light>>();
            break;
        case EventType::SET_SHADER_SPOT_LIGHTS:
            spot_lights = &info.get_val<std::vector<Light>>();
            break;
        case EventType::SET_SHADER_FLASHLIGHT:
            flashlight = &info.get_val<Light>();
            break;
        case EventType::SET_SHADER_TEXTURE_UNIT:
            textureUnit = info.get_val<unsigned int>();
            break;
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

//struct Light {
//    glm::vec3 position;
//    glm::vec3 ambient;
//    glm::vec3 diffuse;
//    glm::vec3 specular;
//    float constant;
//    float linear;
//    float quadratic;
//};

void Shader::send_lights(const char *varName, const std::vector<Light> &light) {
    std::string len_var = "len_";
    len_var += varName;
    glUniform1i(glGetUniformLocation(shaderProgram, len_var.c_str()), static_cast<int>(light.size()));
    std::string array_access, full;
    array_access += varName;
    array_access += "[";
    for(int i = 0; i < light.size(); i++) {
        full = array_access + std::to_string(i) + "]";
        auto send = [&](std::string_view prop, auto func) {
            full += prop;
            func(full.c_str());
            full.erase(full.end() - static_cast<long>(prop.size()), full.end());
        };

//        auto dir = std::string_view(".direction");
//        full += dir;
//        send_vec(full.c_str(), light[i].direction);
//        full.erase(full.end() - static_cast<long>(dir.size()), full.end());
        send(".position", [&, this](auto s) { send_vec(s, light[i].position); });
        send(".direction", [&, this](auto s) { send_vec(s, light[i].direction); });
        send(".ambient", [&, this](auto s) { send_vec(s, light[i].ambient); });
        send(".diffuse", [&, this](auto s) { send_vec(s, light[i].diffuse); });
        send(".specular", [&, this](auto s) { send_vec(s, light[i].specular); });
        send(".cutOff", [&, this](auto s) { send_float(s, light[i].cutOff); });
        send(".outerCutOff", [&, this](auto s) { send_float(s, light[i].outerCutOff); });
        send(".constant", [&, this](auto s) { send_float(s, light[i].constant); });
        send(".linear", [&, this](auto s) { send_float(s, light[i].linear); });
        send(".quadratic", [&, this](auto s) { send_float(s, light[i].quadratic); });
    }
}

void Shader::send_float(const char *varName, float val) {
    glUniform1f(glGetUniformLocation(shaderProgram, varName), val);
}

void Shader::send_flashlight(const Light & l) {
    send_vec("flashlight.position", l.position);
    send_float("flashlight.cutOff", l.cutOff);
    send_float("flashlight.outerCutOff", l.outerCutOff);
    send_vec("flashlight.specular", l.specular);
    send_vec("flashlight.diffuse", l.diffuse);
    send_vec("flashlight.direction", l.direction);
}
