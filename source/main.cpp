#include "glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "plane.h"
#include "sphere.h"
#include "DrawableObject.h"
#include "Scene.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "test", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Scene scene(window);
    int choice = 0;
    switch (choice) {
        case 0:
            auto v = std::vector<float>(&sphere[0], &sphere[sizeof sphere / sizeof sphere[0] - 1]);
            Model sp(v);
            std::vector<DrawableObject> objs { DrawableObject(sp, "noLight"), DrawableObject(sp, "phong"), DrawableObject(sp, "phong"), DrawableObject(sp, "phong") };
            objs[0].set_position( { 3.0, 0.0, 0.0 });
            objs[1].set_position({ -3.0, 0.0, 0.0 });
            objs[2].set_position({ 0.0, 3.0, 0.0 });
            objs[3].set_position({0.0, -3.0, 0.0});
            for (auto o : objs) {
                scene.add_object(std::move(o));
            }

            scene.render();
        break;
    }

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

