#include "Shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

namespace {
GLuint CompileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::ifstream vertexFile(vertexPath);
    std::ifstream fragmentFile(fragmentPath);
    if (!vertexFile || !fragmentFile) {
        std::cout << "ERROR::SHADER::FILE_NOT_FOUND" << std::endl;
    }

    std::stringstream vertexSS, fragmentSS;
    vertexSS << vertexFile.rdbuf();
    fragmentSS << fragmentFile.rdbuf();

    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSS.str());
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSS.str());

    program_ = glCreateProgram();
    glAttachShader(program_, vertexShader);
    glAttachShader(program_, fragmentShader);
    glLinkProgram(program_);

    GLint success;
    glGetProgramiv(program_, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program_, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    if (program_ != 0) {
        glDeleteProgram(program_);
    }
}

Shader::Shader(Shader&&other) noexcept {
    program_ = other.program_;
    other.program_ = 0;
}

Shader& Shader::operator=(Shader&&other) noexcept {
    if (this != &other) {
        if (program_ != 0) {
            glDeleteProgram(program_);
        }
        program_ = other.program_;
        other.program_ = 0;
    }
    return *this;
}

void Shader::Use() const {
    glUseProgram(program_);
}

GLint Shader::GetUniformLocation(const std::string& name) const {
    return glGetUniformLocation(program_, name.c_str());
}