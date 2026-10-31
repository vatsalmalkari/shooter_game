#include "Mesh.hpp"

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position: location 0, first 3 floats
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color: location 1, next 3 floats
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    indexCount_ = static_cast<GLsizei>(indices.size());
}

Mesh::~Mesh() {
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
    }
    if (VBO != 0) {
        glDeleteBuffers(1, &VBO);
    }
    if (IBO != 0) {
        glDeleteBuffers(1, &IBO);
    }
}

void Mesh::Draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Mesh::Mesh(Mesh&& other) noexcept {
    VAO = other.VAO;
    VBO = other.VBO;
    IBO = other.IBO;
    indexCount_ = other.indexCount_;

    other.VAO = 0;
    other.VBO = 0;
    other.IBO = 0;
    other.indexCount_ = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        if (VAO != 0) {
            glDeleteVertexArrays(1, &VAO);
        }
        if (VBO != 0) {
            glDeleteBuffers(1, &VBO);
        }
        if (IBO != 0) {
            glDeleteBuffers(1, &IBO);
        }

        VAO = other.VAO;
        VBO = other.VBO;
        IBO = other.IBO;
        indexCount_ = other.indexCount_;

        other.VAO = 0;
        other.VBO = 0;
        other.IBO = 0;
        other.indexCount_ = 0;
    }
    return *this;
}

