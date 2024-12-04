#ifndef PULSE_RADIUS_CIRCLE_UPDATER_HPP
#define PULSE_RADIUS_CIRCLE_UPDATER_HPP

#include <cmath>
#include <domain/figure/circle/Circle.hpp>
#include <domain/figure/circle/CircleUpdater.hpp>

namespace cust {
class PulseRadiusCircleUpdater : public CircleUpdater {
   protected:
    float radius;
    float pulseAmplitudeRate;
    float frequency;
    timepoint edge = clock::now();

   public:
    PulseRadiusCircleUpdater(float radius, float pulseAmplitudeRate, float frequency)
        : radius{radius}, pulseAmplitudeRate{pulseAmplitudeRate}, frequency{frequency} {}

    void update(Circle& circle, timepoint t) override {
        circle.r = radius *  (1.0f + pulseAmplitudeRate * sin(t));
    }

    void setRadius(float radius) {
        if (0 < radius) {
            this->radius = radius;
        }
    }

    float getRadius() const {
        return radius;
    }

   private:
    float sin(timepoint t) {
        auto time = (t - edge).count();
        return std::sin(frequency * time);
    }
};
}  // namespace cust

#endif