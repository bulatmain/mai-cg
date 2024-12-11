#ifndef FIGURE_DRAWER_HPP
#define FIGURE_DRAWER_HPP


namespace cust {
class Figure;

class FigureDrawer {
   protected:
   public:
    virtual void draw(ptr<Figure const> const& circle) = 0;
};
}  // namespace cust

#endif