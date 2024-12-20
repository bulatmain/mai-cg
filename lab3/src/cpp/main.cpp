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

int main() {
    try {
        ptr<WindowGLFW> window = construct<WindowGLFW>(800, 800, "Scene");

        ptr<ShaderProgram> shader = construct<ShaderProgram>(
            "vertex_shader.glsl", 
            "fragment_shader.glsl" 
        );

        ptr<MeshDrawer> meshDrawer = construct<MeshDrawer>(shader);

        auto blue = glm::vec3(0.184313725f, 0.23529411764705882f, 0.49411764f);

        auto cube = construct<Cube>(
            glm::vec3(0.f, 0.f, -10.0f),
            blue,
            meshDrawer,
            nullptr);

        auto figures = vecp<Figure>{ cube };

        ApplicationGLFW app(
            std::move(window), figures, {});

        app.launch();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
