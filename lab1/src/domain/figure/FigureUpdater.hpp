#ifndef FIGURE_UPDATER_HPP
#define FIGURE_UPDATER_HPP

#include <common/Aliases.hpp>
#include <ctime>

namespace cust {
class Figure;

class FigureUpdater {
   protected:
   public:
    virtual void update(ptr<Figure> const& circle, timepoint t) = 0;
};
}  // namespace cust

#endif