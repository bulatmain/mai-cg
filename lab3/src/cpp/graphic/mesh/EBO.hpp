#ifndef EBO_CLASS_HPP
#define EBO_CLASS_HPP


#include <glad/glad.h>

#include <common/Aliases.hpp>

namespace cust {

class EBO {
   public:
    GLuint ID;
    EBO(vec<GLuint> const& indices);

    void Bind();
    void Unbind();
    void Delete();
};


EBO::EBO(vec<GLuint> const& indices) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}


void EBO::Bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}


void EBO::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void EBO::Delete() {
    glDeleteBuffers(1, &ID);
}

}

#endif