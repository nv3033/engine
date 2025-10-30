#include "ShaderProgramm.h"
#include <iostream>
#include "stb_image.h" // Include for texture loading

ShaderProgramm::ShaderProgramm() {}

ShaderProgramm::~ShaderProgramm() {
    glDeleteProgram(shaderProgram);
}

void ShaderProgramm::init(){
    // 5. Компиляция и линковка шейдеров
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // (Добавьте проверки ошибок компиляции, как в предыдущих примерах)

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // (Добавьте проверки ошибок компиляции)

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // (Добавьте проверки ошибок линковки)

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Получаем location'ы униформ для матриц и текстуры
    glUseProgram(shaderProgram); // Нужно использовать программу, прежде чем получить location'ы униформ
    glUniform1i(glGetUniformLocation(shaderProgram, "uTexture"), 0); // Указываем, что uTexture соответствует GL_TEXTURE0
}

void ShaderProgramm::use(){
        // Использование шейдерной программы
        glUseProgram(shaderProgram);
}

void ShaderProgramm::upload_matrix(glm::mat4 model, glm::mat4 view, glm::mat4 projection){
    // Передача матриц в шейдер

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}