#ifndef COMMON_KEY_PROCESSOR_HPP
#define COMMON_KEY_PROCESSOR_HPP

#include <application/InputProcessor.hpp>
#include <graphic/WindowGLFW.hpp>

namespace cust {
class CommonKeyProcessor : public InputProcessor {
   protected:
    ptr<WindowGLFW> window;

   public:
    CommonKeyProcessor(ptr<WindowGLFW> window) 
        : window{window} {}

    void process() override {
        if (window->isPressed(ESCAPE)) {
            window->setWindowShouldClose();
        }
    }
};
}  // namespace cust

#endif