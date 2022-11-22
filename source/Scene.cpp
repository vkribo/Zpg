#include "Scene.h"

Scene::Scene(GLFWwindow *window) :
        window(window),
        projection()
{
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(window, this);
    auto func = [](GLFWwindow* window, double xpos, double ypos) {
        static_cast<Scene *>(glfwGetWindowUserPointer(window))->mouse_callback(xpos, ypos);
    };

    glfwSetCursorPosCallback(window, func);
    cube.load("assets/cube.obj");

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

    skybox = Skybox(loader.get("skybox"));
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
//        glDepthFunc(GL_LEQUAL);
//        skybox.skybox_shader->use();
//        set_transformations();
//        camera.update_view();
//        skybox.draw();
//        skybox.skybox_shader->unuse();
//        glDepthFunc(GL_LESS);
        for (const auto& o : objects) {
            auto s = loader.get(o->shaderName);
            s->use();
            set_transformations();
            notify(UpdateValueInfo(EventType::SET_SHADER_DIR_LIGHTS, directional_lights));
            notify(UpdateValueInfo(EventType::SET_SHADER_POINT_LIGHTS, point_lights));
            notify(UpdateValueInfo(EventType::SET_SHADER_SPOT_LIGHTS, spot_lights));
            flashlight.position = camera.get_pos();
            flashlight.direction = camera.get_target();
            //spot_lights[0] = flashlight;
            //notify(UpdateValueInfo(EventType::SET_SHADER_FLASHLIGHT, flashlight));
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

