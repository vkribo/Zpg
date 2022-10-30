#include "glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "DrawableObject.h"
#include "Scene.h"

#include "plane.h"
#include "sphere.h"
#include "gift.h"
#include "tree.h"
#include "bushes.h"
namespace actual_plain {
#include "plain.h"
}

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
    int choice = 2;
    switch (choice) {
        case 0: {
            Model sp(sphere, sizeof sphere / sizeof sphere[0]);
            std::vector<DrawableObject> objs;
            objs.emplace_back(sp, "phong");
            objs.emplace_back(sp, "phong");
            objs.emplace_back(sp, "phong");
            objs.emplace_back(sp, "phong");
            objs[0].set_position( { 3.0, 0.0, 0.0 });
            objs[1].set_position({ -3.0, 0.0, 0.0 });
            objs[2].set_position({ 0.0, 3.0, 0.0 });
            objs[3].set_position({0.0, -3.0, 0.0});
            for (auto& o : objs) {
                scene.add_object(std::move(o));
            }

            scene.render();
        }
        break;
        case 1: {
            Model sp(sphere, sizeof sphere / sizeof sphere[0]);
            auto a = DrawableObject(sp, "phongH1NoCheck");
            auto b = DrawableObject(sp, "phongH1WithCheck");
            a.set_position({-3.0, 0.0, 0.0});
            b.set_position({3.0, 0.0, 0.0});
            scene.add_object(std::move(a));
            scene.add_object(std::move(b));
            scene.render();
        }
        break;
        case 2: {
            Model sp(sphere, sizeof sphere / sizeof sphere[0]);
            Model treeModel(tree, sizeof tree / sizeof tree[0]);
            Model giftModel(gift, sizeof gift / sizeof gift[0]);
            Model bushModel(plain, sizeof plain / sizeof plain[0]);
            Model monkeyModel(suziFlat, sizeof suziFlat / sizeof suziFlat[0]);
            Model plainModel(actual_plain::plain, sizeof actual_plain::plain / sizeof actual_plain::plain[0]);
            auto p = DrawableObject(plainModel, "noLight");
            auto s = DrawableObject(sp, "scene");
            s.set_position({0.0, 3.0, -6.0});
            s.set_scale(3.0);
            scene.add_object(std::move(s));
            p.set_scale(1000);
            scene.add_object(std::move(p));
            auto g = DrawableObject(giftModel, "blinn");
            g.set_position({ 1, 0.0, 0.0 });
            scene.add_object(std::move(g));
            auto m = DrawableObject(monkeyModel, "lambert");
            m.set_rotation({ 0.0, 45, 0.0 });
            m.set_position({-2.0, 1, 0.0});
            scene.add_object(std::move(m));
            srand(time(NULL));
            for (int i = 0; i < 100; i++) {
                float x = rand() % 100 - 50;
                float z = rand() % 100 - 50;
                float y = 0;
                auto o = DrawableObject(treeModel, "scene");
                o.set_position({ x, y, z });
                scene.add_object(std::move(o));
            }

            for (int i = 0; i < 2000; i++) {
                float x = rand() % 100 - 50;
                float z = rand() % 100 - 50;
                float y = 0;
                auto o = DrawableObject(bushModel, "scene");
                o.set_position({ x, y, z });
                scene.add_object(std::move(o));
            }

            scene.render();
        }
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

