#ifndef MESH_HPP
#define MESH_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <common/Aliases.hpp>
#include <domain/figure/Figure.hpp>

#include <graphic/mesh/VAO.hpp>
#include <graphic/mesh/VBO.hpp>
#include <graphic/mesh/EBO.hpp>

namespace cust {

class Mesh : public Figure {
   protected:
    VAO vao;

    vec<Vertex> vertices;
    vec<GLuint> indexes;

   public:
    Mesh(vec<Vertex> const& vertices, vec<GLuint> const& indexes, ptr<FigureDrawer> drawer, ptr<FigureUpdater> updater)
        : Figure(drawer, updater), vertices(vertices), indexes(indexes) {

        vao.Bind();
        VBO vbo(vertices);
        EBO ebo(indexes);
        vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
        vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
        vao.Unbind();
        vbo.Unbind();
        ebo.Unbind();

    }

    vec<GLuint> const& getIndexes() const {
        return indexes;
    }

    VAO const& getVAO() const { return vao; }
    GLsizei getVertexCount() const { return vertices.size(); }
};

}  // namespace cust

#endif
