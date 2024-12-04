#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <domain/figure/Figure.hpp>
#include <domain/figure/circle/CircleDrawer.hpp>
#include <domain/figure/circle/CircleUpdater.hpp>

namespace cust {

struct Circle : public Figure {
    float x;
    float y;
    float r;
    ptr<CircleDrawer> drawer;
    ptr<CircleUpdater> updater;

    Circle(float x, float y, float r,
           ptr<CircleDrawer> drawer,
           ptr<CircleUpdater> updater)
        : x{x}, y{y}, r{r}, drawer{drawer}, updater{updater} {}

    void draw() const override {
        drawer->draw(*this);
    }

    void update(timepoint t) override {
        updater->update(*this, t);
    }

    float getRadius() const {
        return r;
    }

    friend class CircleUpdater;
    friend class CircleDrawer;
};

}  // namespace cust

#endif