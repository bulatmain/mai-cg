#ifndef GLFWINITEXCEPTION_HPP
#define GLFWINITEXCEPTION_HPP

#include <common/Exception.hpp>

namespace cust {
class GLFWInitException : public Exception {
   public:
    GLFWInitException(std::string const& msg) : Exception(msg) {}
};
}  // namespace cust

#endif