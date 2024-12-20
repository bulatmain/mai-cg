#ifndef WINDOWGLFW_HPP
#define WINDOWGLFW_HPP

#include <common/Aliases.hpp>
#include <graphic/Keys.hpp>
#include <graphic/exception/GLFWInitException.hpp>
#include <graphic/exception/GLFWWindowInitException.hpp>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace cust {

class WindowGLFW {
   private:
    GLFWwindow* window;
    int width, height;
    str title;

   public:
    WindowGLFW(u16 width, u16 height, str title)
        : width(width), height(height), title(title) {
        if (!glfwInit()) {
            throw GLFWInitException("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (!window) {
            glfwTerminate();
            throw GLFWWindowInitException("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw GLFWInitException("Failed to initialize GLAD");
        }

        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST);

        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        glfwSetKeyCallback(window, keyCallback);
    }

    bool isPressed(Keys key) const {
        return glfwGetKey(window, key) == GLFW_PRESS;
    }

    bool shouldNotClose() const {
        return !glfwWindowShouldClose(window);
    }

    void setWindowShouldClose() const {
        glfwSetWindowShouldClose(window, true);
    }

    void startIteration() const {
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void endIteration() const {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ~WindowGLFW() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

   private:
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }
};

}  // namespace cust

#endif
