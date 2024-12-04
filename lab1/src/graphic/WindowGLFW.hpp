#ifndef WINDOWGLFW_HPP
#define WINDOWGLFW_HPP

#include <GLFW/glfw3.h>

#include <common/Aliases.hpp>
#include <graphic/exception/GLFWInitException.hpp>
#include <graphic/exception/GLFWWindowInitException.hpp>
#include <graphic/Keys.hpp>

#include <iostream>

namespace cust {

class WindowGLFW {
   protected:
    GLFWwindow* window;

   public:
    WindowGLFW(u16 width, u16 height, str title) {
        if (!glfwInit()) {
            throw GLFWInitException("Error: can not init GLFW");
        }
        window = glfwCreateWindow(
            width,
            height,
            title.c_str(),
            NULL,
            NULL);
        if (!window) {
            glfwTerminate();
            throw GLFWWindowInitException("Error: can not init window");
        }
        glfwMakeContextCurrent(window);
        glfwSetKeyCallback(window, keyCallback);
        glClearColor(255.0, 255.0, 255.0, 1);
    }

    bool isPressed(Keys key) {
        return glfwGetKey(window, key) == GLFW_PRESS;
    }

    bool shouldNotClose() {
        return !glfwWindowShouldClose(window);
    }

    void setWindowShouldClose() {
        glfwSetWindowShouldClose(window, true);
    }

    void startIteration() {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width, height, 0, 0, 1);
        glMatrixMode(GL_MODELVIEW);
    }

    void endIteration() {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ~WindowGLFW() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

   protected:
    static void keyCallback(GLFWwindow* w, int key, int scancode, int action, int mods) {
        if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(w, GL_TRUE);
        }
    }
};
}  // namespace cust

#endif