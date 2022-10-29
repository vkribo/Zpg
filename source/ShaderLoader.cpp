#include "ShaderLoader.h"

#include <filesystem>
#include <fstream>
#include <algorithm>
#include <iostream>

void ShaderLoader::load() {
    std::unordered_map<std::string, std::vector<std::pair<std::string, ShaderType>>> temp;
    std::string file;
    auto add = [&temp, &file](auto& ext, auto& path, auto type) {
        temp[file.substr(0, file.size() - ext.size())]
                .push_back(std::make_pair(read_file(path), type));
    };

    for (const auto& f : std::filesystem::directory_iterator("shaders")) {
        if (f.is_directory()) {
            continue;
        }

        file = f.path().filename().native();
        std::string ext = ".frag";
        if (file.ends_with(ext)) {
            add(ext, f.path().native(), ShaderType::FRAGMENT);
            continue;
        }

        ext = ".vert";
        if (file.ends_with(ext)) {
            add(ext, f.path().native(), ShaderType::VERTEX);
            continue;
        }
    }

    for (const auto& e : temp) {
        std::cout << e.first << std::endl;
    }

    std::transform(
            temp.begin(),
            temp.end(),
            std::inserter(shaders, shaders.end()),
            [](auto p)
            {
                return std::make_pair(p.first, std::make_unique<Shader>(std::move(p.second)));
            });
}

Shader* ShaderLoader::get(const std::string &name) {
    return shaders.at(name).get();
}

std::string ShaderLoader::read_file(const std::string &path) {
    std::ifstream is(path);
    std::stringstream buffer;
    buffer << is.rdbuf();
    return buffer.str();
}

const std::unordered_map<std::string, std::unique_ptr<Shader>> &ShaderLoader::shaders_ref() {
    return shaders;
}
