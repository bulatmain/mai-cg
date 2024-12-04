#ifndef TIMER_HPP
#define TIMER_HPP

#include <common/Aliases.hpp>

namespace cust {
class Timer {
   protected:
    nanoseconds offset;

   public:
    Timer() : Timer(nanoseconds{}) {}
    Timer(nanoseconds offset) : offset{offset} {}
    timepoint now() {
        return offset + clock::now();
    }
};

}  // namespace cust

#endif