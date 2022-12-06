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

            Light l;
            l.position = glm::vec3(0.0f, 0.0, 0.0f);
            l.ambient = l.specular = l.diffuse = glm::vec3(1.0);
            scene.add_point_light(l, true);

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
            Model house;
            house.load("assets/model.obj");
            DrawableObject house_object(house, "phong");
            Texture t;
            t.load("assets/test.png");
            house_object.set_texture(t);
            //house_object.add_animation(std::make_unique<RepeatedLineMovement>(glm::vec3(1,1,1), 10));
            auto& house_ref = scene.add_object(std::move(house_object));
            t.load("assets/test.png");
            Model tree;
            tree.load("assets/tree/tree.obj");
            DrawableObject tree_obj(tree, "phong");
            t.load("assets/tree/tree.png");
            tree_obj.set_texture(t);
            tree_obj.set_position(glm::vec3(20, 0.0, 0.0));
            tree_obj.set_scale(0.2);
            auto& treeRef = scene.add_object(std::move(tree_obj));
            Model zombie;
            zombie.load("assets/zombie/zombie.obj");
            t.load("assets/zombie/zombie.png");
            DrawableObject zobmie_obj(zombie, "phong");
            zobmie_obj.set_texture(t);
            auto& zombie_ref = scene.add_object(std::move(zobmie_obj));
            zombie_ref.set_position(glm::vec3(10, 0.0, 0.0));
            zombie_ref.add_animation(std::make_unique<RotateAroundObject>(treeRef));
            DrawableObject zombie_obj2(zombie, "phong");
            zombie_obj2.set_texture(t);
            auto& zombie2Ref = scene.add_object(std::move(zombie_obj2));
            zombie2Ref.set_position(glm::vec3(8, 0.0, 0.0));
            zombie2Ref.add_animation(std::make_unique<RepeatedLineMovement>(glm::vec3(0, 0, 1), 10));
            Model terrain;
            terrain.load("assets/terrain/teren.obj");
            t.load("assets/grass.png");
            DrawableObject terrainObj(terrain, "phong");
            terrainObj.set_texture(t);
            scene.add_object(std::move(terrainObj));
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

