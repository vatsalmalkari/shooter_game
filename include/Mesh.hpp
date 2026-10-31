#pragma once

#include <vector>
#include <glad/glad.h>

// Mesh.hpp
class Mesh {
public:
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh();
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;
    void Draw() const;
private:
    GLuint VAO = 0, VBO = 0, IBO = 0;
    GLsizei indexCount_ = 0;
};