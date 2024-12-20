#ifndef FIGURE_HPP
#define FIGURE_HPP

#include <common/Aliases.hpp>
#include <domain/figure/FigureDrawer.hpp>
#include <domain/figure/FigureUpdater.hpp>

namespace cust {
struct Figure : std::enable_shared_from_this<Figure> {

    ptr<FigureDrawer> drawer;
    ptr<FigureUpdater> updater;

    Figure(ptr<FigureDrawer> drawer, ptr<FigureUpdater> updater)
        : drawer(drawer), updater(updater) {}

    Figure() : Figure(nullptr, nullptr) {}
    
    virtual void draw() const {
        if (drawer) {
            drawer->draw(getPtr());
        }
    }

    virtual void update(timepoint t) {
        if (updater) {
            updater->update(getPtr(), t);
        }
    }

    virtual ptr<Figure> getPtr() {
        return shared_from_this();
    }

    virtual ptr<Figure const> getPtr() const {
        return shared_from_this();
    }
};
}  // namespace cust

#endif