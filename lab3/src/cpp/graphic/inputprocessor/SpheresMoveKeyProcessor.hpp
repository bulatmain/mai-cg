#ifndef SPHERES_MOVE_KEY_PROCESSOR_HPP
#define SPHERES_MOVE_KEY_PROCESSOR_HPP

#include <application/InputProcessor.hpp>
#include <graphic/mesh/sphere/Sphere.hpp>
#include <graphic/WindowGLFW.hpp>

namespace cust {

class SpheresMoveKeyProcessor : public InputProcessor {
   protected:
    ptr<WindowGLFW> window;

    vecp<Sphere> spheres;

    ptr<Sphere> current;

   public:
    SpheresMoveKeyProcessor(ptr<WindowGLFW> window, vecp<Sphere> const& spheres)
        : window{window}, spheres(spheres) {
        if (spheres.empty()) {
            throw std::invalid_argument("Error: no spheres specifyed!");
        }
        current = spheres[0];
    }

    void process() override {
        if (window->isPressed(ZERO)) {
            current = spheres[0];
        } else if (window->isPressed(ONE)) {
            current = spheres[1];
        } else if (window->isPressed(FRONT)) {
            current->move(glm::vec3(0.f, 0.f, -0.1f));
        } else if (window->isPressed(BACK)) {
            current->move(glm::vec3(0.f, 0.f, 0.1f));
        } else if (window->isPressed(LEFT)) {
            current->move(glm::vec3(-0.1f, 0.f, 0.0f));
        } else if (window->isPressed(RIGHT)) {
            current->move(glm::vec3(0.1f, 0.f, 0.0f));
        }
    }
};

}  // namespace cust

#endif