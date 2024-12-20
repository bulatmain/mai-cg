#ifndef ApplicationGLFW_HPP
#define ApplicationGLFW_HPP

#include <application/Application.hpp>
#include <graphic/WindowGLFW.hpp>

namespace cust {

class ApplicationGLFW : public Application {
   protected:
    ptr<WindowGLFW> window;

   public:
    ApplicationGLFW(
        ptr<WindowGLFW>&& window, 
        vecp<Figure> figures,
        vecp<InputProcessor> InputProcessors
        ) : Application(), window(window) {
        Application::setFigures(figures);
        Application::setInputProcessors(InputProcessors);
    }

    void iterate() override {
        window->startIteration();
        Application::iterate();
        window->endIteration();
    }

   protected:
    bool keepGoing() override {
        return window->shouldNotClose();
    }
};

}  // namespace cust

#endif