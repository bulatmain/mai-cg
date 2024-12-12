#ifndef GLFWWINDOWINITEXCEPTION_HPP
#define GLFWWINDOWINITEXCEPTION_HPP

#include <common/Exception.hpp>

namespace cust {
class GLFWWindowInitException : public Exception {
   public:
    GLFWWindowInitException(std::string const& msg) : Exception(msg) {}
};
}  // namespace cust

#endif