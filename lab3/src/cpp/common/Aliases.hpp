#ifndef ALIASES_HPP
#define ALIASES_HPP

#include <chrono>
#include <cinttypes>
#include <memory>
#include <vector>

namespace cust {

// Chrono time aliases
using namespace std::literals;
using clock = std::chrono::system_clock;
using timepoint = clock::time_point;

using nanoseconds = std::chrono::nanoseconds;

// Integer types aliases
using u64 = uint64_t;
using i64 = int64_t;
using u16 = uint16_t;
using i16 = int16_t;

// Poiner aliases

template <class T>
using ptr = std::shared_ptr<T>;

template <class T, class... Args>
ptr<T> construct(Args... args) {
    return std::make_shared<T>(args...);
}

// String alias
using str = std::string;

// Container aliases

template <class T>
using vec = std::vector<T>;

template <class T>
using vecp = vec<ptr<T>>;

}  // namespace cust

#endif