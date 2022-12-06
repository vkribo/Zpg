#pragma once

#include <string>
#include <glad.h>
#include <vector>
#include <glm/ext/matrix_float4x4.hpp>
#include "Observer.h"
#include "Light.h"


enum class ShaderType {
    VERTEX,
    FRAGMENT
};

class Shader : public Observer {
    std::vector<std::pair<std::string, ShaderType>> shaders;
    unsigned int shaderProgram = -1;
    bool inUse = false;

    const glm::mat4* model = nullptr;
    const glm::mat4* projection = nullptr;
    const glm::mat4* view = nullptr;
    const glm::vec3* viewPos = nullptr;
    const std::vector<Light>* dir_lights = nullptr;
    const std::vector<Light>* point_lights = nullptr;
    const std::vector<Light>* spot_lights = nullptr;
    const Light* flashlight = nullptr;
    unsigned int textureUnit;


    static unsigned int create_shader(const std::string& source, ShaderType type);
    void create_shaders();
    void send_float(const char* varName, float val);
    void send_matrix(const char* varName, const glm::mat4 &matrix);
    void send_vec(const char* varName, const glm::vec3 &vec);
    void send_lights(const char *varName, const std::vector<Light> &light);
    void send_flashlight(const Light& flashlight);
public:
    explicit Shader(std::vector<std::pair<std::string, ShaderType>> shaders);
    unsigned int get_program_id() const;
    ~Shader();
    void use();
    void unuse();
    void update(const UpdateInfo& info) override;
};

