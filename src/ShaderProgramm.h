#ifndef SHADERPROGRAMM_H
#define SHADERPROGRAMM_H

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgramm {
public:
    ShaderProgramm();
    ~ShaderProgramm();

    void init();
    void use();

    void upload_matrix(glm::mat4 model, glm::mat4 view, glm::mat4 projection);

private:


// --- Исходный код вершинного шейдера ---
const char* vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;

    out vec2 TexCoord;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
        TexCoord = aTexCoord;
    }
)glsl";

// --- Исходный код фрагментного шейдера ---
const char* fragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 FragColor;
    in vec2 TexCoord;

    uniform sampler2D uTexture;

    void main()
    {
        // Переворот текстуры по оси Y
        vec2 flippedTexCoord = vec2(TexCoord.x, 1.0 - TexCoord.y);

        vec4 texColor = texture(uTexture, flippedTexCoord);

        if (texColor.a < 0.1) {
            discard; // Пропускаем пиксели с низкой прозрачностью
        }
        FragColor = texColor; // Используем цвет с альфа-каналом
    }
)glsl";

    unsigned int shaderProgram = glCreateProgram();
};

#endif
