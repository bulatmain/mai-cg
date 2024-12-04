#ifndef CIRCLE_DRAWER_HPP
#define CIRCLE_DRAWER_HPP


namespace cust {
class Circle;

class CircleDrawer {
   protected:
   public:
    virtual void draw(Circle const& circle) = 0;
};
}  // namespace cust

#endif