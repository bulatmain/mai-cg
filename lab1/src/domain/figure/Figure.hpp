#ifndef FIGURE_HPP
#define FIGURE_HPP

#include <common/Aliases.hpp>

namespace cust {
struct Figure {
    virtual void draw() const = 0;
    virtual void update(timepoint t) = 0;
};
}  // namespace cust

#endif