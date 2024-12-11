#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <domain/figure/Figure.hpp>
#include <domain/figure/circle/CircleDrawer.hpp>
#include <domain/figure/circle/CircleUpdater.hpp>

#include <iostream>

namespace cust {

struct Circle : public Figure {
    float x;
    float y;
    float r;

    Circle(float x, float y, float r,
           ptr<CircleDrawer> drawer,
           ptr<CircleUpdater> updater)
        : Figure(drawer, updater), x{x}, y{y}, r{r} {}

    float getRadius() const {
        return r;
    }
};

}  // namespace cust

#endif