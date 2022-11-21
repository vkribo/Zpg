#pragma once

#include <string>
#include <vector>

class Texture {
    unsigned int texture = -1;
public:
    Texture() = default;
    void load(const std::string &file_path);
    void load_cubemap(const std::vector<std::string> &file_paths);
    void bind() const;
    unsigned int get_texture_id() const;
};