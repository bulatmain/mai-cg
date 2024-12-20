#ifndef PYRAMID_HPP
#define PYRAMID_HPP

#include <graphic/mesh/Mesh.hpp>
#include <vector>
#include <array>

namespace cust {

class Pyramid : public Mesh {
   public:
    Pyramid(glm::vec3 center, glm::vec3 color, 
         ptr<FigureDrawer> drawer, ptr<FigureUpdater> updater)
        : Mesh(generateVertices(center, color), getIndexes(), drawer, updater) {}

   private:
    static vec<Vertex> generateVertices(glm::vec3 center, glm::vec3 color) {

        std::array<glm::vec3, 8> baseVertices = {
            glm::vec3(-0.5f, -0.5f, -0.5f), 
            glm::vec3(-0.5f, -0.5f,  0.5f),
            glm::vec3(0.5f, -0.5f,  0.5f), 
            glm::vec3(0.5f, -0.5f, -0.5f),
            glm::vec3(0.0f,  0.5f,  0.0f)
        };

        vec<Vertex> vertices;

        for (auto const& vertice : baseVertices) {
            vertices.push_back({
                vertice + center,
                color
            });
        }
        return vertices;
    }

    static vec<GLuint> getIndexes() {
        return vec<GLuint> {        
            3, 0, 1,
            3, 1, 2,
            4, 1, 0,
            4, 2, 1,
            4, 3, 2,
            4, 0, 3,
        };
    }

};

}  // namespace cust

#endif
