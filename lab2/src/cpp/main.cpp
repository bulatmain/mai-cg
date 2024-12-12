#define GLFW_INCLUDE_NONE

#include <graphic/ApplicationGLFW.hpp>
#include <graphic/WindowGLFW.hpp>
#include <graphic/drawer/MeshDrawer.hpp>
#include <graphic/inputprocessor/SpheresMoveKeyProcessor.hpp>
#include <graphic/mesh/cube/Cube.hpp>
#include <graphic/mesh/cylinder/Cylinder.hpp>
#include <graphic/mesh/pyramid/Pyramid.hpp>
#include <graphic/mesh/sphere/Sphere.hpp>
#include <graphic/shader/ShaderProgram.hpp>
#include <iostream>

using namespace cust;

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Error: enter shader source pathes\n";
        return -1;
    }
    try {
        ptr<WindowGLFW> window = construct<WindowGLFW>(800, 800, "Scene");

        ptr<ShaderProgram> shader = construct<ShaderProgram>(
            argv[1], argv[2]);

        ptr<MeshDrawer> meshDrawer = construct<MeshDrawer>(shader);

        auto cylinder = construct<Cylinder>(
            glm::vec3(-3.0f, -2.0f, -10.0f),
            glm::vec3(0.f, 0.f, 1.f),
            50,
            2,
            1,
            meshDrawer,
            nullptr);

        auto pyramid = construct<Pyramid>(
            glm::vec3(0.0f, -1.0f, -10.0f),
            glm::vec3(1.f, 0.f, 0.f),
            meshDrawer,
            nullptr);

        auto cube = construct<Cube>(
            glm::vec3(3.0f, -2.0f, -10.0f),
            glm::vec3(0.f, 1.f, 0.f),
            meshDrawer,
            nullptr);

        auto sphere1 = construct<Sphere>(
            glm::vec3(-1.f, 2.0f, -10.0f),
            glm::vec3(1.f, 0.9f, 0.f),
            50,
            50,
            0.5,
            meshDrawer,
            nullptr);

        auto sphere2 = construct<Sphere>(
            glm::vec3(1.f, 2.0f, -10.0f),
            glm::vec3(1.f, 0.8f, 0.f),
            50,
            50,
            0.5,
            meshDrawer,
            nullptr);

        auto figures = vec<Figure>{
            cube,
            pyramid,
            cylinder,
            sphere1,
            sphere2};

        auto processors = vec<InputProcessor>{
            construct<SpheresMoveKeyProcessor>(
                window, vec<Sphere>{sphere1, sphere2})};

        ApplicationGLFW app(
            std::move(window), figures, processors);

        app.launch();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
