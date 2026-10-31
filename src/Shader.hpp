#pragma once
#include <string>
#include <glad/glad.h>

class Shader{
public:

    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&&other) noexcept;
    Shader& operator=(Shader&&other) noexcept;
    
    void Use() const;
    GLint GetUniformLocation(const std::string& name) const;

private:
    GLuint program_ = 0;
};