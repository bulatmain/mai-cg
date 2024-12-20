#ifndef CUBE_HPP
#define CUBE_HPP

#include <graphic/mesh/Mesh.hpp>
#include <vector>
#include <array>

namespace cust {

class Cube : public Mesh {
   public:
    Cube(glm::vec3 center, glm::vec3 color, 
         ptr<FigureDrawer> drawer, ptr<FigureUpdater> updater)
        : Mesh(generateVertices(center, color), getIndexes(), drawer, updater) {}

   private:
    static vec<Vertex> generateVertices(glm::vec3 center, glm::vec3 color) {
        std::array<glm::vec3, 8> baseVertices = {
            glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f),
            glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(-0.5f,  0.5f, -0.5f),
            glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.5f, -0.5f,  0.5f),
            glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(-0.5f,  0.5f,  0.5f)
        };

        vec<Vertex> vertices;
        for (auto const& vertice : baseVertices) {
            vertices.push_back({ vertice + center, color });
        }
        return vertices;
    }

    static vec<GLuint> getIndexes() {
        return vec<GLuint>{
            0, 1, 2, 2, 3, 0,  // Задняя
            4, 5, 6, 6, 7, 4,  // Передняя
            3, 2, 6, 6, 7, 3,  // Верхняя
            0, 1, 5, 5, 4, 0,  // Нижняя
            1, 5, 6, 6, 2, 1,  // Правая
            0, 4, 7, 7, 3, 0   // Левая
        };
    }
};

}  // namespace cust

#endif
