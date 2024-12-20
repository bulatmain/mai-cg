#ifndef VBO_CLASS_HPP
#define VBO_CLASS_HPP

#include <glad/glad.h>

#include <common/Aliases.hpp>
#include <glm/glm.hpp>

namespace cust {

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

class VBO {
   public:
    GLuint ID;
    VBO(vec<Vertex> const& vertices);

    void Bind();
    void Unbind();
    void Delete();
};

VBO::VBO(vec<Vertex> const& vertices) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

void VBO::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() {
    glDeleteBuffers(1, &ID);
}

}  // namespace cust

#endif