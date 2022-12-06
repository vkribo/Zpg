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
//    int choice = 2;
//    switch (choice) {
//        case 0: {
//            Model sp(sphere, sizeof sphere / sizeof sphere[0]);
//            std::vector<DrawableObject> objs;
//            objs.emplace_back(sp, "phong");
//            objs.emplace_back(sp, "phong");
//            objs.emplace_back(sp, "phong");
//            objs.emplace_back(sp, "phong");
//            objs[0].set_position( { 3.0, 0.0, 0.0 });
//            objs[1].set_position({ -3.0, 0.0, 0.0 });
//            objs[2].set_position({ 0.0, 3.0, 0.0 });
//            objs[3].set_position({0.0, -3.0, 0.0});
//            for (auto& o : objs) {
//                scene.add_object(std::move(o));
//            }
//
//            scene.render();
//        }
//        break;
//        case 1: {
//            Model sp(sphere, sizeof sphere / sizeof sphere[0]);
//            auto a = DrawableObject(sp, "phongH1NoCheck");
//            auto b = DrawableObject(sp, "phongH1WithCheck");
//            a.set_position({-3.0, 0.0, 0.0});
//            b.set_position({3.0, 0.0, 0.0});
//            scene.add_object(std::move(a));
//            scene.add_object(std::move(b));
//            scene.render();
//        }
//        break;
//        case 2: {
//            Model sp(sphere, sizeof sphere / sizeof sphere[0]);
//            Model treeModel(tree, sizeof tree / sizeof tree[0]);
//            Model giftModel(gift, sizeof gift / sizeof gift[0]);
//            Model bushModel(plain, sizeof plain / sizeof plain[0]);
//            Model monkeyModel(suziFlat, sizeof suziFlat / sizeof suziFlat[0]);
//            Model plainModel(actual_plain::plain, sizeof actual_plain::plain / sizeof actual_plain::plain[0]);
//            auto p = DrawableObject(plainModel, "noLight");
//            auto s = DrawableObject(sp, "phong");
//            s.set_position({0.0, 3.0, -6.0});
//            s.set_scale(3.0);
//            scene.add_object(std::move(s));
//            p.set_scale(1000);
//            scene.add_object(std::move(p));
//            auto g = DrawableObject(giftModel, "phong");
//            g.set_position({ 1, 0.0, 0.0 });
//            scene.add_object(std::move(g));
//            auto m = DrawableObject(monkeyModel, "phong");
//            m.set_rotation({ 0.0, 45, 0.0 });
//            m.set_position({-2.0, 1, 0.0});
//            scene.add_object(std::move(m));
//            srand(time(NULL));
//            for (int i = 0; i < 100; i++) {
//                float x = rand() % 100 - 50;
//                float z = rand() % 100 - 50;
//                float y = 0;
//                auto o = DrawableObject(treeModel, "phong");
//                o.set_position({ x, y, z });
//                scene.add_object(std::move(o));
//            }
//
//            for (int i = 0; i < 100; i++) {
//                float x = rand() % 100 - 50;
//                float z = rand() % 100 - 50;
//                float y = 0;
//                auto o = DrawableObject(bushModel, "phong");
//                o.set_position({ x, y, z });
//                scene.add_object(std::move(o));
//            }
//
//            scene.render();
//        }
//        break;
//    }
    Model house;
    house.load("assets/model.obj");
    DrawableObject house_object(house, "phong");
    Texture t;
    t.load("assets/grass.png");
    house_object.set_texture(t);
    //house_object.add_animation(std::make_unique<RepeatedLineMovement>(glm::vec3(1,1,1), 10));
    auto& house_ref = scene.add_object(std::move(house_object));
    t.load("assets/test.png");
    DrawableObject second_house(house, "phong");
    second_house.set_texture(t);
    second_house.set_position(glm::vec3(20, 0.0, 0.0));
    auto& second_house_ref = scene.add_object(std::move(second_house));
    second_house_ref.add_animation(std::make_unique<RotateAroundObject>(house_ref, 10));
    Model terrain;
    terrain.load("assets/terrain/teren.obj");
    t.load("assets/grass.png");
    DrawableObject terrainObj(terrain, "phong");
    terrainObj.set_texture(t);
    scene.add_object(std::move(terrainObj));
    scene.render();

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

