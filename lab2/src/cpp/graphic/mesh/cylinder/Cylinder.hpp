#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include <graphic/mesh/Mesh.hpp>
#include <vector>
#include <cmath>

namespace cust {

class Cylinder : public Mesh {
   public:
    Cylinder(glm::vec3 center, glm::vec3 color, int segments, float height, float radius, 
             ptr<FigureDrawer> drawer, ptr<FigureUpdater> updater)
        : Mesh(generateVertices(center, color, segments, height, radius), 
               getIndexes(segments), drawer, updater) {}

   private:
    static std::vector<Vertex> generateVertices(glm::vec3 center, glm::vec3 color, int segments, float height, float radius) {
        std::vector<Vertex> vertices;

        // Верхняя и нижняя центральные вершины
        vertices.push_back({ center + glm::vec3(0, height / 2, 0), color });  // Верхний центр
        vertices.push_back({ center - glm::vec3(0, height / 2, 0), color });  // Нижний центр

        // Вершины цилиндра
        for (int i = 0; i < segments; ++i) {
            float theta = 2.0f * M_PI * i / segments;
            float x = radius * cos(theta);
            float z = radius * sin(theta);

            // Верхние и нижние грани
            vertices.push_back({ center + glm::vec3(x, height / 2, z), color });
            vertices.push_back({ center + glm::vec3(x, -height / 2, z), color });
        }
        return vertices;
    }

    static std::vector<GLuint> getIndexes(int segments) {
        std::vector<GLuint> indices;

        // Верхняя и нижняя крышки
        for (int i = 0; i < segments; ++i) {
            int next = (i + 1) % segments;

            // Верхняя крышка
            indices.push_back(0);  // Верхний центр
            indices.push_back(2 + i * 2); 
            indices.push_back(2 + next * 2); 

            // Нижняя крышка
            indices.push_back(1);  // Нижний центр
            indices.push_back(3 + next * 2); 
            indices.push_back(3 + i * 2); 
        }

        // Боковые грани
        for (int i = 0; i < segments; ++i) {
            int next = (i + 1) % segments;

            GLuint v0 = 2 + i * 2;
            GLuint v1 = 3 + i * 2;
            GLuint v2 = 2 + next * 2;
            GLuint v3 = 3 + next * 2;

            // Два треугольника боковой грани
            indices.insert(indices.end(), { v0, v2, v1 });
            indices.insert(indices.end(), { v1, v2, v3 });
        }

        return indices;
    }
};

}  // namespace cust

#endif
