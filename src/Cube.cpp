#include "Cube.h"
#include <iostream>
#include <vector>

static const std::vector<float> vertices = {
    // positions          // texcoords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

Cube::Cube() : position(0.0f) {
    init();
}

Cube::~Cube() {
    // Освобождение ресурсов (например, VAO и VBO)
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

void Cube::init() {
    // Настройка шейдеров
    setupShaders();

    // Создание VAO и VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

    // Настройка указателей для вершин
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

void Cube::draw(const glm::mat4& view, const glm::mat4& projection) {
    glUseProgram(shaderProgram);

    // Установка матриц вида и проекции
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Установка позиции
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Рисование куба
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 вершин для куба
    glBindVertexArray(0);
}

void Cube::setPosition(const glm::vec3& newPosition) {
    position = newPosition;
}

void Cube::setupShaders() {
    const char* vertexShaderSource = R"glsl(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec2 aTexCoord;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        out vec2 TexCoord;

        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            TexCoord = aTexCoord;
        }
    )glsl";

    const char* fragmentShaderSource = R"glsl(
        #version 330 core
        in vec2 TexCoord;
        out vec4 FragColor;

        // Если не используете текстуры, можно убрать и заменить на const color
        uniform sampler2D texture1;
        uniform vec3 overrideColor; // если нужно перекрасить (по умолчанию vec3(1.0))

        void main() {
            vec4 texColor = texture(texture1, TexCoord);
            // Если текстура полностью прозрачна, можно отдавать альфу; иначе комбинируем
            FragColor = texColor * vec4(overrideColor, 1.0);
        }
    )glsl";

    auto compileShader = [](GLenum type, const char* src) -> GLuint {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLint logLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
            std::string log(logLen, '\0');
            glGetShaderInfoLog(shader, logLen, nullptr, &log[0]);
            const char* t = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
            std::cerr << t << " SHADER COMPILATION ERROR:\n" << log << std::endl;
            glDeleteShader(shader);
            return 0;
        }
        return shader;
    };

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    if (vertexShader == 0) return;

    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    if (fragmentShader == 0) {
        glDeleteShader(vertexShader);
        return;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint linkSuccess = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        GLint logLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
        std::string log(logLen, '\0');
        glGetProgramInfoLog(program, logLen, nullptr, &log[0]);
        std::cerr << "SHADER PROGRAM LINK ERROR:\n" << log << std::endl;
        glDeleteProgram(program);
        program = 0;
    }

    // Шейдеры больше не нужны после линковки
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Если раньше был шейдер — удалить
    if (shaderProgram != 0) {
        glDeleteProgram(shaderProgram);
    }
    shaderProgram = program;

    // Задаём значения по умолчанию для uniform'ов (опционально)
    if (shaderProgram != 0) {
        glUseProgram(shaderProgram);
        GLint texLoc = glGetUniformLocation(shaderProgram, "texture1");
        if (texLoc >= 0) glUniform1i(texLoc, 0); // GL_TEXTURE0
        GLint colorLoc = glGetUniformLocation(shaderProgram, "overrideColor");
        if (colorLoc >= 0) glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);
        glUseProgram(0);
    }
}
