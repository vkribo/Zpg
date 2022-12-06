#include "Scene.h"

#include <glm/ext/matrix_projection.hpp>
#include <chrono>

Scene::Scene(GLFWwindow *window) :
        window(window),
        projection()
{
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(window, this);
    auto func = [](GLFWwindow* window, double xpos, double ypos) {
        static_cast<Scene *>(glfwGetWindowUserPointer(window))->mouse_callback(xpos, ypos);
    };

    auto userMouseClick = [](GLFWwindow* window, int button, int action, int mods) {
        static_cast<Scene *>(glfwGetWindowUserPointer(window))->mouse_clicked(button, action, mods);
    };

    glfwSetMouseButtonCallback(window, userMouseClick);
    glfwSetCursorPosCallback(window, func);
    cube.load("assets/cube.obj");

    tree.load("assets/tree/tree.obj");
    treeTex.load("assets/tree/tree.png");

    Light l;
    l.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    l.ambient = glm::vec3(0.2f);
    l.specular = glm::vec3(0.5f);
    l.diffuse = glm::vec3(0.5f);
    directional_lights.push_back(l);
    l.position = glm::vec3(0.0f, 2.0f, 12.0f);
    l.ambient = l.specular = l.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    add_point_light(l, true);
    flashlight = l;
    flashlight.cutOff = glm::cos(glm::radians(12.5f));
    flashlight.outerCutOff = glm::cos(glm::radians(17.5f));
    //spot_lights.push_back(flashlight);
}

void Scene::render() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    loader.load();
    for (const auto& s : loader.shaders_ref()) {
        attach(s.second.get());
        camera.attach(s.second.get());
    }

    notify(UpdateValueInfo(EventType::SET_SHADER_DIR_LIGHTS, directional_lights));
    notify(UpdateValueInfo(EventType::SET_SHADER_POINT_LIGHTS, point_lights));
    notify(UpdateValueInfo(EventType::SET_SHADER_SPOT_LIGHTS, spot_lights));

    skybox = Skybox(loader.get("skybox"));
    auto start = std::chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(window))
    {
        int newWidth, newHeight;
        glfwGetWindowSize(window, &newWidth, &newHeight);
        if (scr_width != newWidth || newHeight != scr_height) {
            scr_width = newWidth;
            scr_height = newHeight;
            projection = glm::perspective(glm::radians(45.0f), (float)scr_width / (float)scr_height, 0.1f, 1000.0f);
            notify(UpdateValueInfo(EventType::SET_SHADER_PROJECTION, projection));
        }

        auto current = std::chrono::high_resolution_clock::now();
        auto deltaTime = current - start;
        if (std::chrono::duration_cast<std::chrono::microseconds>(deltaTime).count() > 1666) {
            for (auto& o : objects) {
                o->step_animation();
            }
        }

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        skybox.draw();
        flashlight.position = camera.get_pos();
        flashlight.direction = camera.get_target();
        for (int i = 0; i < objects.size(); i++) {
            const auto& o = objects[i];
            auto s = loader.get(o->shaderName);
            notify(UpdateValueInfo(EventType::SET_SHADER_FLASHLIGHT, flashlight));
            glStencilFunc(GL_ALWAYS, i, 0xFF);
            o->draw_object(s);
        }

        glStencilFunc(GL_ALWAYS, -1, 0xFF);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

DrawableObject & Scene::add_object(DrawableObject obj) {
    auto u = std::make_unique<DrawableObject>(std::move(obj));
    auto& res = *u;
    objects.push_back(std::move(u));
    //notify(UpdateValueInfo<Shader*>(EventType::SET_SHADER, &shader));
    return res;
}

void Scene::processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    const float cameraSpeed = 0.2f; // adjust accordingly
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

void Scene::add_point_light(const Light &light, bool cube_representation) {
    if (cube_representation) {
        DrawableObject cube_obj(cube, "noLight");
        cube_obj.set_scale(0.2);
        cube_obj.set_position(light.position);
        add_object(std::move(cube_obj));
    }

    point_lights.push_back(light);
}

std::pair<unsigned int, glm::vec3> Scene::get_clicked_obj() {
    GLbyte color[4];
    GLfloat depth;
    GLuint index;

    GLint x = lastX;
    GLint y = lastY;

    int newy = scr_height - y;
    glReadPixels(x, newy, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
    glReadPixels(x, newy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    glReadPixels(x, newy, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

    glm::vec3 screenX = glm::vec3(x, newy, depth);
    glm::mat4 view = camera.get_view();
    glm::mat4 projection = this->projection;
    glm::vec4 viewPort = glm::vec4(0, 0, scr_width, scr_height);
    glm::vec3 pos = glm::unProject(screenX, view, projection, viewPort);
    printf("unProject [%f,%f,%f]\n", pos.x, pos.y, pos.z);
    return std::make_pair(index, pos);
}

void Scene::mouse_clicked(int button, int action, int mods) {
    if (action != 1) {
        return;
    }

    if (button == 0) {
        plant_tree();
        return;
    }

    if (button == 1) {
        delete_obj();
        return;
    }
}

void Scene::plant_tree() {
    auto coords = get_clicked_obj().second;
    auto obj = DrawableObject(tree, "phong");
    obj.set_texture(treeTex);
    obj.set_scale(0.3);
    obj.set_position(coords);
    add_object(std::move(obj));
}

void Scene::delete_obj() {
    auto pair = get_clicked_obj();
    auto i = pair.first;
    if (glm::distance(pair.second, camera.get_pos()) > 100.0) {
        return;
    }

    if (i != -1 && i < objects.size()) {
        objects.erase(objects.begin() + i);
    }
}

