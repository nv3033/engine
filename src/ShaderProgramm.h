#pragma once
#include <string>
#include <glad/glad.h>

class ShaderProgramm {
public:
    ShaderProgramm() : ID(0) {}
    ~ShaderProgramm();

    bool createFromSources(const char* vertexSrc, const char* fragmentSrc, std::string &outError);
    void use() const;
    GLuint id() const { return ID; }

    // Утилита для установки матрицы 4x4
    void setMat4(const char* name, const float* mat4) const;

private:
    GLuint ID;
    bool compileShader(GLuint shader, std::string &outError) const;
    bool linkProgram(GLuint vs, GLuint fs, std::string &outError);
};
