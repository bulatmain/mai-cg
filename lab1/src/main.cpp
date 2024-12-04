#include <domain/figure/circle/Circle.hpp>
#include <graphic/ApplicationGLFW.hpp>
#include <graphic/drawer/BresenhamsCircleDrawer.hpp>
#include <graphic/inputprocessor/CircleRadiusUpdateKeyProcessor.hpp>
#include <graphic/inputprocessor/CommonKeyProcessor.hpp>
#include <graphic/updater/PulseRadiusCircleUpdater.hpp>

using namespace cust;

int main() {
    float const radius = 50, rate = 0.2, freq = 10e-9;

    auto drawer = construct<BresenhamsCircleDrawer>();
    auto updater = construct<PulseRadiusCircleUpdater>(radius, rate, freq);

    auto figures = vec<Figure>{
        construct<Circle>(400, 400, radius, drawer, updater)};

    auto window = construct<WindowGLFW>(800, 800, str("Circle!"));

    auto inputProcessors = vec<InputProcessor>{
        construct<CommonKeyProcessor>(window),
        construct<CircleRadiusUpdateKeyProcessor>(window, updater, 5)};

    ApplicationGLFW app(std::move(window), figures, inputProcessors);

    app.launch();

    return 0;
}