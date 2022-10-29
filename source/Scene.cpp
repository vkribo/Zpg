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
                                   "uniform vec3 viewPos;\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);\n"
                                   "   float ambientStrength = 0.2f;\n"
                                   "   vec3 ambient = ambientStrength * lightColor;\n"
                                   "   vec3 lightPos = vec3(0.f, 0.f, 0.f);\n"
                                   "   vec3 norm = normalize(Normal);\n"
                                   "   vec3 lightDir = normalize(lightPos - FragPos);\n"
                                   "   float diff = max(dot(norm, lightDir), 0.0);\n"
                                   "   vec3 objectColor = vec3(0.f, 0.3f, 0.5f);\n"
                                   "   vec3 diffuse = diff * lightColor;\n"
                                   "   float specularStrength = 0.5;\n"
                                   "   vec3 viewDir = normalize(viewPos - FragPos);\n"
                                   "   vec3 reflectDir = reflect(-lightDir, norm);\n"
                                   "   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
                                   "   vec3 specular = (dot(norm, lightDir) < 0.0)? vec3(0.0) : specularStrength * spec * lightColor;\n"
                                   "   vec3 result = (ambient + diffuse + specular) * objectColor;\n"
                                   "   FragColor = vec4(result, 1.0);\n"
                                   "}\n\0";

const char *fragmentShaderSourceH1WithCheck = "#version 330 core\n"
                                   "in vec3 FragPos;\n"
                                   "in vec3 Normal;\n"
                                   "uniform vec3 viewPos;\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);\n"
                                   "   float ambientStrength = 0.2f;\n"
                                   "   vec3 ambient = ambientStrength * lightColor;\n"
                                   "   vec3 lightPos = vec3(0.f, 0.f, 0.f);\n"
                                   "   vec3 norm = normalize(Normal);\n"
                                   "   vec3 lightDir = normalize(lightPos - FragPos);\n"
                                   "   float diff = max(dot(norm, lightDir), 0.0);\n"
                                   "   vec3 objectColor = vec3(0.f, 0.3f, 0.5f);\n"
                                   "   vec3 diffuse = diff * lightColor;\n"
                                   "   float specularStrength = 0.5;\n"
                                   "   vec3 viewDir = normalize(viewPos - FragPos);\n"
                                   "   vec3 reflectDir = reflect(-lightDir, norm);\n"
                                   "   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1);\n"
                                   "   vec3 specular = (dot(norm, lightDir) < 0.0)? vec3(0.0) : specularStrength * spec * lightColor;\n"
                                   "   vec3 result = (ambient + diffuse + specular) * objectColor;\n"
                                   "   FragColor = vec4(result, 1.0);\n"
                                   "}\n\0";

Scene::Scene(GLFWwindow *window) :
        window(window),
        //shader({ std::make_pair(vertexShaderSource, ShaderType::VERTEX), std::make_pair(fragmentShaderSource, ShaderType::FRAGMENT) }),
        projection()
{
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(window, this);
    auto func = [](GLFWwindow* window, double xpos, double ypos) {
        static_cast<Scene *>(glfwGetWindowUserPointer(window))->mouse_callback(xpos, ypos);
    };

    glfwSetCursorPosCallback(window, func);
}

void Scene::set_transformations() {
    notify(UpdateValueInfo(EventType::SET_SHADER_PROJECTION, projection));
}

void Scene::render() {
    glEnable(GL_DEPTH_TEST);
    loader.load();
    for (const auto& s : loader.shaders_ref()) {
        attach(s.second.get());
        camera.attach(s.second.get());
    }
    int scr_width, scr_height;
    bool first = true;
    while (!glfwWindowShouldClose(window))
    {
        glfwGetWindowSize(window, &scr_width, &scr_height);
        projection = glm::perspective(glm::radians(45.0f), (float)scr_width / (float)scr_height, 0.1f, 100.0f);

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (const auto& o : objects) {
            auto s = loader.get(o->shaderName);
            s->use();
            set_transformations();
            camera.update_view();
            o->shader = s;
            o->draw_object();
            s->unuse();
        }

        if (first) {
            camera.update_view();
            first = false;
        }

        //for (const auto& s : loader.shaders_ref()) {
          //  detach(s.second.get());
            //camera.detach(s.second.get());
        //}

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Scene::add_object(DrawableObject obj) {
    auto u = std::make_unique<DrawableObject>(std::move(obj));
    objects.push_back(std::move(u));
    //notify(UpdateValueInfo<Shader*>(EventType::SET_SHADER, &shader));
}

void Scene::processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    const float cameraSpeed = 0.05f; // adjust accordingly
    auto cameraPos = camera.get_pos();
    auto cameraFront = camera.get_target();
    auto cameraUp = camera.get_up();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    camera.set_pos(cameraPos);
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
    camera.set_target(glm::normalize(direction));
}

void Scene::load_shaders() {

}

