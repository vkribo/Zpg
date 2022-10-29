#pragma once

#include "glad.h"
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

#include "Shader.h"
#include "Camera.h"
#include "DrawableObject.h"
#include "ShaderLoader.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

class Scene : public Subject {
    GLFWwindow* window;
    ShaderLoader loader;
    std::vector<std::unique_ptr<DrawableObject>> objects;
    Camera camera;
    glm::mat4 projection;
    //glm::mat4 view;
    float lastX = 400, lastY = 300;
    bool firstMouse = true;
    float pitch = 0, yaw = -90;
    void load_shaders();
public:
    explicit Scene(GLFWwindow* window);
    void set_transformations();
    void render();
    void add_object(DrawableObject obj);
    void processInput(GLFWwindow *window);
    void mouse_callback(float xpos, float ypos);
};