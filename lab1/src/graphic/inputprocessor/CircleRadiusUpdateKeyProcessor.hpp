#ifndef CIRCLE_RADIUS_UPDATE_KEY_PROCESSOR_HPP
#define CIRCLE_RADIUS_UPDATE_KEY_PROCESSOR_HPP

#include <application/InputProcessor.hpp>
#include <graphic/WindowGLFW.hpp>
#include <graphic/updater/PulseRadiusCircleUpdater.hpp>

namespace cust {

class CircleRadiusUpdateKeyProcessor : public InputProcessor {
   protected:
    ptr<WindowGLFW> window;
    ptr<PulseRadiusCircleUpdater> updater;
    float dr;

   public:
    CircleRadiusUpdateKeyProcessor(
        ptr<WindowGLFW> window,
        ptr<PulseRadiusCircleUpdater> updater,
        float dr)
        : window{window}, updater{updater}, dr{dr} {}

    void process() override {
        if (window->isPressed(UP)) {
            updater->setRadius(updater->getRadius() + dr);
        } else if (window->isPressed(DOWN)) {
            updater->setRadius(updater->getRadius() - dr);
        }
    }
};

}  // namespace cust

#endif