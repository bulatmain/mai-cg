#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <cmath>
#include <graphic/mesh/Mesh.hpp>
#include <vector>

namespace cust {

class Sphere : public Mesh {
   public:
    Sphere(glm::vec3 center, glm::vec3 color, int latitudeSegments, int longitudeSegments, float radius,
           ptr<FigureDrawer> drawer, ptr<FigureUpdater> updater)
        : Mesh(generateVertices(center, color, latitudeSegments, longitudeSegments, radius),
               getIndexes(latitudeSegments, longitudeSegments), drawer, updater) {}

   public:
    void move(glm::vec3 direction) {
        for (auto& vertice : vertices) {
            vertice.position += direction;
        }
        vao.Bind();
        VBO vbo(vertices);
        vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
        vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
        vbo.Unbind();
        vao.Unbind();
    }

   private:
    static vec<Vertex> generateVertices(glm::vec3 center, glm::vec3 color, int latSeg, int lonSeg, float radius) {
        vec<Vertex> vertices;

        for (int lat = 0; lat <= latSeg; ++lat) {
            float theta = M_PI * lat / latSeg;  // От 0 до PI (от южного полюса к северному)
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            for (int lon = 0; lon <= lonSeg; ++lon) {
                float phi = 2.0f * M_PI * lon / lonSeg;  // От 0 до 2PI
                float sinPhi = sin(phi);
                float cosPhi = cos(phi);

                // Вычисление позиции вершины
                glm::vec3 position = glm::vec3(
                                         radius * sinTheta * cosPhi,
                                         radius * cosTheta,
                                         radius * sinTheta * sinPhi) +
                                     center;

                vertices.push_back({position, color});
            }
        }
        return vertices;
    }

    static vec<GLuint> getIndexes(int latSeg, int lonSeg) {
        vec<GLuint> indices;

        for (int lat = 0; lat < latSeg; ++lat) {
            for (int lon = 0; lon < lonSeg; ++lon) {
                int current = lat * (lonSeg + 1) + lon;
                int next = current + lonSeg + 1;

                indices.push_back(current);
                indices.push_back(next);
                indices.push_back(current + 1);

                indices.push_back(current + 1);
                indices.push_back(next);
                indices.push_back(next + 1);
            }
        }
        return indices;
    }
};

}  // namespace cust

#endif
