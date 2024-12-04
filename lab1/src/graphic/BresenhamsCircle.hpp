// #ifndef BRESENHAMS_CIRCLE_HPP
// #define BRESENHAMS_CIRCLE_HPP

// #include <cmath>
// #include <domain/Circle.hpp>
// #include <functional>
// #include <glm/matrix.hpp>

// namespace cust {
// class BresenhamsCircle : public Circle {
//    protected:
//     float x;
//     float y;
//     float radius;
//     std::function<glm::vec3(std::time_t)> updateFunc;

//    public:
//     BresenhamsCircle(glm::vec3 const& conf, std::function<glm::vec3(std::time_t)> const& updateFunction)
//         : updateFunc{updateFunc} {
//         setConfiguration(conf);
//     }

//     void draw() const override {
//         float x = 0.0f, y = radius;
//         float d = 3.0f - 2.0f * radius;
//     }

//     void update(std::time_t timestamp) override {
//         auto conf = updateFunc(timestamp);
//         setConfiguration(conf);
//     }

//    protected:
//     void drawSegments

//         void
//         setConfiguration(glm::vec3 const& conf) {
//         x = conf[0];
//         y = conf[1];
//         radius = conf[2];
//     }
// };
// }  // namespace cust

// #endif