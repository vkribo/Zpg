#pragma once

#include <unordered_map>
#include <memory>
#include "Shader.h"

class ShaderLoader {
    std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
    static std::string read_file(const std::string& path);
public:
    void load();
    Shader* get(const std::string& name);
    const std::unordered_map<std::string, std::unique_ptr<Shader>>& shaders_ref();
};