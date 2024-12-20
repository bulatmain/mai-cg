#ifndef MESH_DRAWER_HPP
#define MESH_DRAWER_HPP

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <domain/figure/FigureDrawer.hpp>
#include <graphic/mesh/Mesh.hpp>
#include <graphic/shader/ShaderProgram.hpp>

namespace cust {

class MeshDrawer : public FigureDrawer {
   protected:
    ptr<ShaderProgram> shader;

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    GLuint modelLoc;
    GLuint viewLoc;
    GLuint projLoc;


   public:
    MeshDrawer(ptr<ShaderProgram> shaderProgram)
        : shader(shaderProgram) {
        view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
        proj = glm::perspective(glm::radians(45.0f), (float)(800 / 800), 0.1f, 100.f);
        auto program = shader->getProgram();
        modelLoc = glGetUniformLocation(program, "model");
        viewLoc = glGetUniformLocation(program, "view");
        projLoc = glGetUniformLocation(program, "proj");
    }

    virtual void draw(ptr<Figure const> const& figure) override {

        auto mesh = std::dynamic_pointer_cast<const Mesh>(figure);
        if (!mesh) return;

        shader->use();

        auto vao = mesh->getVAO();

        vao.Bind();

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        glDrawElements(GL_TRIANGLES, mesh->getIndexes().size(), GL_UNSIGNED_INT, 0);

    }

    GLuint getProgram() const { return shader->getProgram(); }
};

}  // namespace cust

#endif
