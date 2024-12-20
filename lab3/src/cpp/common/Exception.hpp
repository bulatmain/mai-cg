#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <stdexcept>

namespace cust {
class Exception : std::exception {
   protected:
    std::string const msg;

   public:
    Exception(std::string const& msg) : msg{msg} {}

    char const* what() const noexcept override {
        return msg.c_str();
    }
};
}  // namespace cust

#endif