#ifndef BRESENHAMS_CIRCLE_DRAWER_HPP
#define BRESENHAMS_CIRCLE_DRAWER_HPP

#include <GLFW/glfw3.h>

#include <domain/figure/circle/Circle.hpp>
#include <domain/figure/circle/CircleDrawer.hpp>

namespace cust {
class BresenhamsCircleDrawer : public CircleDrawer {
   public:
    void draw(ptr<Figure const> const& figure) override {
        try {
            auto circle = std::dynamic_pointer_cast<Circle const>(figure);
            float x = 0, y = circle->r;
            float delta = 3 - 2 * circle->r;
            glBegin(GL_LINE_LOOP);
            glColor3f(0, 0, 1);
            putPixels(circle->x, circle->y, x, y);
            while (x <= y) {
                if (delta > 0) {
                    y--;
                    delta += 4 * (x - y) + 10;
                } else {
                    delta += 4 * x + 6.0;
                }
                ++x;
                putPixels(circle->x, circle->y, x, y);
            }
            glEnd();
        } catch (std::bad_cast const& e) {}
    }

   protected:
    void putPixels(float x0, float y0, float dx, float dy) {
        glVertex2f(x0 + dx, y0 + dy);
        glVertex2f(x0 + dx, y0 - dy);
        glVertex2f(x0 - dx, y0 + dy);
        glVertex2f(x0 - dx, y0 - dy);

        glVertex2f(x0 + dy, y0 + dx);
        glVertex2f(x0 + dy, y0 - dx);
        glVertex2f(x0 - dy, y0 + dx);
        glVertex2f(x0 - dy, y0 - dx);
    }
};
}  // namespace cust

#endif