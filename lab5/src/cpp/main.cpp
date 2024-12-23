#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <cmath>
#include <iostream>
#include <memory>
#include <random>
#include <thread>
#include <vector>

// Constants
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// Helper functions
struct Vec3 {
    float x, y, z;

    Vec3 operator+(const Vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }
    Vec3 operator-(const Vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }
    Vec3 operator*(float scalar) const { return {x * scalar, y * scalar, z * scalar}; }
    Vec3 operator/(float scalar) const { return {x / scalar, y / scalar, z / scalar}; }
    Vec3 normalized() const {
        float mag = std::sqrt(x * x + y * y + z * z);
        return {x / mag, y / mag, z / mag};
    }
    float dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    Vec3 cross(const Vec3& v) const {
        return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
    }
};

struct Ray {
    Vec3 origin;
    Vec3 direction;
};

struct Figure {
    virtual bool intersect(const Ray& ray, float& t) const = 0;
    virtual Vec3 getColor() const = 0;
    virtual Vec3 getNormal(Vec3 const& hit_point) const = 0;
};

struct Sphere : public Figure {
    Vec3 center;
    float radius;
    Vec3 color;

    Sphere(Vec3 const& center,
           float radius,
           Vec3 const& color = {1.f, 0.f, 0.f})
        : center(center), radius(radius), color(color) {}

    bool intersect(const Ray& ray, float& t) const override {
        Vec3 oc = ray.origin - center;
        float a = ray.direction.dot(ray.direction);
        float b = 2.f * oc.dot(ray.direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;
        if (discriminant < 0) return false;
        t = (-b - std::sqrt(discriminant)) / (2.f * a);
        return t > 0;
    }

    Vec3 getColor() const override {
        return color;
    }

    Vec3 getNormal(Vec3 const& hit_point) const override {
        return (hit_point - center).normalized();
    }
};

struct Plane : public Figure {
    Vec3 point;
    Vec3 normal;

    Plane(Vec3 const& point, Vec3 const& normal)
        : point(point), normal(normal) {}

    bool intersect(const Ray& ray, float& t) const override {
        float denom = normal.dot(ray.direction);
        if (std::fabs(denom) > 1e-6) {
            t = (point - ray.origin).dot(normal) / denom;
            return t >= 0;
        }
        return false;
    }

    Vec3 getColor() const override {
        return {0.6f, 0.6f, 0.6f};
    }

    Vec3 getNormal(Vec3 const& hit_point) const override {
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

class Renderer : public std::enable_shared_from_this<Renderer> {
   private:
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;
    vecp<Figure> figures;

   public:
    static ptr<Renderer> construct(std::mt19937 const& gen,
                                   std::uniform_real_distribution<float> const& dis,
                                   vecp<Figure> const& figures) {
        return ptr<Renderer>(new Renderer(gen, dis, figures));
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
             vecp<Figure> const& figures)
        : gen(std::move(gen)), dis(std::move(dis)), figures(figures) {}

    // Random number generation
    Vec3 random_in_hemisphere(const Vec3& normal) {
        while (true) {
            Vec3 random_vec = {dis(gen) * 2.f - 1.f, dis(gen) * 2.f - 1.f, dis(gen) * 2.f - 1.f};
            if (random_vec.dot(random_vec) <= 1.f && random_vec.dot(normal) > 0.f) {
                return random_vec.normalized();
            }
        }
    }

    Vec3 trace(const Ray& ray, int depth = 0) {
        float t_min = INFINITY;
        Vec3 color = {0.f, 0.f, 0.f};

        // Check sphere intersections
        for (const auto& figure : figures) {
            float t;
            if (figure->intersect(ray, t) && t < t_min) {
                t_min = t;
                Vec3 hit_point = ray.origin + ray.direction * t;
                Vec3 normal = figure->getNormal(hit_point);
                color = figure->getColor();

                if (depth < 2) {  // Limit recursion depth
                    Vec3 indirect_light = {0.f, 0.f, 0.f};
                    for (unsigned int i = 0; i < SAMPLES; ++i) {
                        Vec3 random_dir = random_in_hemisphere(normal);
                        Ray indirect_ray = {hit_point, random_dir};
                        indirect_light = indirect_light + trace(indirect_ray, depth + 1);
                    }
                    color = color * 0.8f + (indirect_light / static_cast<float>(SAMPLES)) * 0.2f;
                }
            }
        }

        // If no hit, return sky color
        if (t_min == INFINITY) {
            return Vec3{0.5f, 0.7f, 1.f} * (0.5f + 0.5f * ray.direction.y);  // Sky color gradient
        }

        // Add ambient lighting
        Vec3 ambient_light = {0.2f, 0.2f, 0.2f};      // Low-level ambient light
        color = color * 0.8f + ambient_light * 0.2f;  // Blend object color with ambient light

        return color;
    }

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

                Vec3 color = renderer->trace(ray);

                int index = (y * WIDTH + x) * 3;
                framebuffer[index] = static_cast<unsigned char>(std::clamp(color.x, 0.f, 1.f) * 255);
                framebuffer[index + 1] = static_cast<unsigned char>(std::clamp(color.y, 0.f, 1.f) * 255);
                framebuffer[index + 2] = static_cast<unsigned char>(std::clamp(color.z, 0.f, 1.f) * 255);
            }
        }
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
        construct<Sphere>(Vec3{0.f, -1.f, -8.f}, 2.f, Vec3{1.f, 0.f, 0.f}),
        construct<Sphere>(Vec3{-2.f, 0.f, -6.f}, 1.f, Vec3{0.f, 1.f, 0.f}),
        construct<Sphere>(Vec3{2.f, 0.f, -6.f}, 1.f, Vec3{0.f, 0.f, 1.f}),
        construct<Plane>(Vec3{0.f, 1.f, 0.f}, Vec3{0.f, -1.f, 0.f})};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.f, 1.f);

    auto renderer = Renderer::construct(gen, dis, figures);

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
