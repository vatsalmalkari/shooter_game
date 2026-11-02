#pragma once
#include <string>
#include <glad/glad.h>

class Texture {
public:
    explicit Texture(const std::string& path);
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    void Bind(unsigned int unit = 0) const;

private:
    GLuint id_ = 0;
};