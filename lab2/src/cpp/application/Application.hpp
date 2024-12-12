#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <application/InputProcessor.hpp>
#include <application/Timer.hpp>
#include <domain/figure/Figure.hpp>

namespace cust {
class Application {
   protected:
    vec<Figure> figures;
    vec<InputProcessor> inputProcessors;
    ptr<Timer> timer;

   public:
    Application() : timer(construct<Timer>()) {}

    void setFigures(vec<Figure> figures) {
        this->figures = std::move(figures);
    }

    void setInputProcessors(vec<InputProcessor> inputProcessors) {
        this->inputProcessors = inputProcessors;
    }

    void launch() {
        while (keepGoing()) {
            iterate();
        }
    }

   protected:
    virtual void iterate() {
        auto timestamp = timer->now();
        for (auto const& processor : inputProcessors) {
            processor->process();
        }
        for (auto const& figure : figures) {
            figure->update(timestamp);
            figure->draw();
        }
    }

    virtual bool keepGoing() = 0;
};

}  // namespace cust

#endif