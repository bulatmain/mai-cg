#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <cmath>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <random>
#include <thread>
#include <vector>

// Constants
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// Helper functions
struct vec3 {
    float x;
    float y;
    float z;

    vec3 operator+(const vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }
    vec3 operator-(const vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }
    vec3 operator-() const { return {-x, -y, -z}; }
    vec3 operator+() const { return {+x, +y, +z}; }
    vec3 operator*(float scalar) const { return {x * scalar, y * scalar, z * scalar}; }
    vec3 operator/(float scalar) const { return {x / scalar, y / scalar, z / scalar}; }

    vec3 normalized() const {
        float mag = std::sqrt(x * x + y * y + z * z);
        return vec3{x / mag, y / mag, z / mag};
    }
    float dot(const vec3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }
    vec3 cross(const vec3& v) const {
        return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
    }
};

struct Ray {
    vec3 origin;
    vec3 direction;
};

struct Figure {
    virtual bool intersect(const Ray& ray, float& t) const = 0;
    virtual vec3 getColor() const = 0;
    virtual vec3 getNormal(vec3 const& hit_point) const = 0;
};

struct Sphere : public Figure {
    vec3 center;
    float radius;
    vec3 color;

    Sphere(vec3 const& center,
           float radius,
           vec3 const& color = {1.f, 0.f, 0.f})
        : center(center), radius(radius), color(color) {}

    bool intersect(const Ray& ray, float& t) const override {
        vec3 oc = ray.origin - center;
        float a = ray.direction.dot(ray.direction);
        float b = 2.f * oc.dot(ray.direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;
        if (discriminant < 0) return false;
        t = (-b - std::sqrt(discriminant)) / (2.f * a);
        return t > 0;
    }

    vec3 getColor() const override {
        return color;
    }

    vec3 getNormal(vec3 const& hit_point) const override {
        return (hit_point - center).normalized();
    }
};

struct Plane : public Figure {
    vec3 point;
    vec3 normal;

    Plane(vec3 const& point, vec3 const& normal)
        : point(point), normal(normal) {}

    bool intersect(const Ray& ray, float& t) const override {
        float denom = normal.dot(ray.direction);
        if (std::fabs(denom) > 1e-6) {
            t = (point - ray.origin).dot(normal) / denom;
            return t >= 0;
        }
        return false;
    }

    vec3 getColor() const override {
        return {0.6f, 0.6f, 0.6f};
    }

    vec3 getNormal(vec3 const& hit_point) const override {
        return normal;
    }
};

const unsigned int SAMPLES = 2;  // Number of random samples for distributed lighting
const float FOV = 90.f;         // Field of view in degrees

template <class T>
using ptr = std::shared_ptr<T>;

template <class T>
using vec = std::vector<T>;

template <class T>
using vecp = vec<ptr<T>>;

template <class T, class... Args>
ptr<T> construct(Args... args) {
    return std::make_shared<T>(args...);
}

struct Light {
    vec3 position;   // Light source position
    vec3 color;      // Light color (white)
    float shininess; // Specular shininess

};

class Renderer : public std::enable_shared_from_this<Renderer> {
   private:
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;
    vecp<Figure> figures;
    Light light;


   public:
    static ptr<Renderer> construct(std::mt19937 const& gen,
                                   std::uniform_real_distribution<float> const& dis,
                                   vecp<Figure> const& figures,
                                   Light const& light) {
        return ptr<Renderer>(new Renderer(gen, dis, figures, light));
    }

    void render(unsigned char* framebuffer) {
        const int thread_count = std::thread::hardware_concurrency();
        vec<std::thread> threads;
        int rows_per_thread = HEIGHT / thread_count;

        for (int t = 0; t < thread_count; ++t) {
            int start = t * rows_per_thread;
            int end = (t == thread_count - 1) ? HEIGHT : start + rows_per_thread;
            auto thread = std::thread(renderSection, shared_from_this(), start, end, framebuffer);
            threads.emplace_back(std::move(thread));
        }

        for (auto& thread : threads) thread.join();
    }

   private:
    Renderer(std::mt19937 const& gen,
             std::uniform_real_distribution<float> const& dis,
             vecp<Figure> const& figures,
             Light const& light)
        : gen(std::move(gen)), dis(std::move(dis)), figures(figures), light(light) {}

    // Render scene
    static void renderSection(ptr<Renderer> renderer, int start, int end, unsigned char* framebuffer) {
        float aspect_ratio = static_cast<float>(WIDTH) / HEIGHT;
        float scale = std::tan((FOV * 0.5f) * M_PI / 180.f);

        for (unsigned int y = start; y < end; ++y) {
            for (unsigned int x = 0; x < WIDTH; ++x) {
                float u = (2 * (x + 0.5f) / WIDTH - 1) * aspect_ratio * scale;
                float v = (1 - 2 * (y + 0.5f) / HEIGHT) * scale;

                Ray ray;
                ray.origin = {0.f, 0.f, 0.f};
                ray.direction = {u, v, -1.f};
                ray.direction = ray.direction.normalized();

                vec3 color = renderer->trace(ray);

                int index = (y * WIDTH + x) * 3;
                framebuffer[index] = static_cast<unsigned char>(std::clamp(color.x, 0.f, 1.f) * 255);
                framebuffer[index + 1] = static_cast<unsigned char>(std::clamp(color.y, 0.f, 1.f) * 255);
                framebuffer[index + 2] = static_cast<unsigned char>(std::clamp(color.z, 0.f, 1.f) * 255);
            }
        }
    }

    vec3 trace(const Ray& ray, int depth = 0) {
        float t_min = INFINITY;
        vec3 color = {0.f, 0.f, 0.f};

        for (const auto& figure : figures) {
            float t = 0.f;
            if (figure->intersect(ray, t) && t < t_min) {
                t_min = t;
                vec3 hit_point = ray.origin + ray.direction * t;
                vec3 normal = figure->getNormal(hit_point);
                vec3 view_dir = -ray.direction;

                // Lighting calculations
                vec3 light_dir = (light.position - hit_point).normalized();
                vec3 reflect_dir = (light_dir - normal * 2.f * light_dir.dot(normal)).normalized();

                float diff = std::max(normal.dot(light_dir), 0.f);  // Diffuse
                float spec = std::pow(std::max(view_dir.dot(reflect_dir), 0.f), light.shininess);  // Specular

                // Ambient light
                vec3 ambient = figure->getColor() * 0.2f; // Увеличьте базовый фон
                color = figure->getColor() * diff + light.color * spec + ambient;
            }
        }

        // If no hit, return sky color
        if (t_min == INFINITY) {
            return vec3{0.5f, 0.7f, 1.f} * (0.5f + 0.5f * ray.direction.y);
        }

        return color;
    }
};

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ray Tracing", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Scene objects
    vecp<Figure> figures{
        construct<Sphere>(vec3{0.f, -1.f, -8.f}, 2.f, vec3{1.f, 0.f, 0.f}),
        construct<Sphere>(vec3{-2.f, 0.f, -6.f}, 1.f, vec3{0.f, 1.f, 0.f}),
        construct<Sphere>(vec3{2.f, 0.f, -6.f}, 1.f, vec3{0.f, 0.f, 1.f}),
        construct<Plane>(vec3{0.f, 1.f, 0.f}, vec3{0.f, -1.8f, 0.f})};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.f, 1.f);

    Light light(
        {-10.f, -10.f, 10.f},
        {1.f, 1.f, 1.f},
        32.f
    );

    auto renderer = Renderer::construct(gen, dis, figures, light);

    unsigned char* framebuffer = new unsigned char[WIDTH * HEIGHT * 3];

    while (!glfwWindowShouldClose(window)) {
        renderer->render(framebuffer);

        glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, framebuffer);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete[] framebuffer;
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
