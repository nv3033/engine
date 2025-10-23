#include "ShaderProgramm.h"
#include <iostream>

ShaderProgramm::~ShaderProgramm() {
    if (ID) glDeleteProgram(ID);
}

bool ShaderProgramm::compileShader(GLuint shader, std::string &outError) const {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        outError = infoLog;
        return false;
    }
    return true;
}

bool ShaderProgramm::linkProgram(GLuint vs, GLuint fs, std::string &outError) {
    ID = glCreateProgram();
    glAttachShader(ID, vs);
    glAttachShader(ID, fs);
    glLinkProgram(ID);

    GLint success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(ID, 1024, nullptr, infoLog);
        outError = infoLog;
        glDeleteProgram(ID);
        ID = 0;
        return false;
    }
    return true;
}

bool ShaderProgramm::createFromSources(std::string &outError) {
    const char* vertexSrc = R"glsl(
        #version 330 core
        layout (location=0) in vec3 aPos;
        layout (location=1) in vec3 aColor;
        out vec3 vColor;
        uniform mat4 uMVP;
        void main() {
            vColor = aColor;
            gl_Position = uMVP * vec4(aPos, 1.0);
        }
    )glsl";

    const char* fragmentSrc = R"glsl(
        #version 330 core
        in vec3 vColor;
        out vec4 FragColor;
        void main() {
            FragColor = vec4(vColor, 1.0);
        }
    )glsl";
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexSrc, nullptr);
    glCompileShader(vs);
    if (!compileShader(vs, outError)) { glDeleteShader(vs); return false; }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentSrc, nullptr);
    glCompileShader(fs);
    if (!compileShader(fs, outError)) { glDeleteShader(vs); glDeleteShader(fs); return false; }

    if (!linkProgram(vs, fs, outError)) { glDeleteShader(vs); glDeleteShader(fs); return false; }

    glDeleteShader(vs);
    glDeleteShader(fs);
    return true;
}

void ShaderProgramm::use() const {
    if (ID) glUseProgram(ID);
}

void ShaderProgramm::setMat4(const char* name, const float* mat4) const {
    if (!ID) return;
    GLint loc = glGetUniformLocation(ID, name);
    if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, mat4);
}
