#include "Scene.h"

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 0) in vec3 aNormal;\n"
                                 "uniform mat4 gModel;\n"
                                 "uniform mat4 gProjection;\n"
                                 "uniform mat4 gView;\n"
                                 "out vec3 FragPos;\n"
                                 "out vec3 Normal;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = gProjection * gView * (gModel * vec4(aPos.x, aPos.y, aPos.z, 1.0));\n"
                                 "   FragPos = vec3(gModel * vec4(aPos, 1.0));\n"
                                 "   Normal = aNormal;\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "in vec3 FragPos;\n"
                                   "in vec3 Normal;\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);\n"
                                   "   float ambientStrength = 0.1f;\n"
                                   "   vec3 ambient = ambientStrength * lightColor;\n"
                                   "   vec3 lightPos = vec3(100.0f, 100.0f, 100.0f);\n"
                                   "   vec3 norm = normalize(Normal);\n"
                                   "   vec3 lightDir = normalize(lightPos - FragPos);\n"
                                   "   float diff = max(dot(norm, lightDir), 0.0);\n"
                                   "   vec3 objectColor = vec3(0.f, 0.3f, 0.5f);\n"
                                   "   vec3 diffuse = diff * lightColor;\n"
                                   "   vec3 result = (ambient + diffuse) * objectColor;\n"
                                   "   FragColor = vec4(result, 1.0);"
                                   "}\n\0";

Scene::Scene(GLFWwindow *window) :
        window(window),
        shader({ std::make_pair(vertexShaderSource, ShaderType::VERTEX), std::make_pair(fragmentShaderSource, ShaderType::FRAGMENT) }),
        projection(),
        view()
{
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(window, this);
    auto func = [](GLFWwindow* window, double xpos, double ypos) {
        static_cast<Scene *>(glfwGetWindowUserPointer(window))->mouse_callback(xpos, ypos);
    };

    glfwSetCursorPosCallback(window, func);
}

void Scene::set_transformations() {
    glUniformMatrix4fv(glGetUniformLocation(shader.get_program_id(), "gProjection"), 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader.get_program_id(), "gView"), 1, GL_FALSE, &view[0][0]);
}

void Scene::render() {
    glEnable(GL_DEPTH_TEST);
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);


        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glClearDepth(1);
        view = glm::lookAt(camera.pos, camera.pos + camera.target, camera.up);
        set_transformations();
        notify({ EventType::DRAW });

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Scene::add_object(DrawableObject obj) {
    auto u = std::make_unique<DrawableObject>(std::move(obj));
    attach(u.get());
    objects.push_back(std::move(u));
    notify(UpdateValueInfo<Shader*>(EventType::SET_SHADER, &shader));
}

void Scene::processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    const float cameraSpeed = 0.05f; // adjust accordingly
    auto& cameraPos = camera.pos;
    auto& cameraFront = camera.target;
    auto& cameraUp = camera.up;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void Scene::mouse_callback(float xpos, float ypos) {
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera.target = glm::normalize(direction);
}
