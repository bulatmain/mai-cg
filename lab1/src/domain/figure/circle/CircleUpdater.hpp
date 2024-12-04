#ifndef CIRCLE_UPDATER_HPP
#define CIRCLE_UPDATER_HPP

#include <common/Aliases.hpp>
#include <ctime>

namespace cust {
class Circle;

class CircleUpdater {
   protected:
   public:
    virtual void update(Circle& circle, timepoint t) = 0;
};
}  // namespace cust

#endif