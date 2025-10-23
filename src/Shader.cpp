#include <string> // Включение библиотеки для работы со строками
#include <glad/glad.h> // Включение библиотеки GLAD для работы с OpenGL
#include "Shader.h" // Включение заголовочного файла Shader для определения класса Shader

// Конструктор класса Shader
Shader::Shader(const std::string &vertexSource, const std::string &fragmentSource) {
    // Создание вершинного шейдера
    unsigned int vertexShaderId = CreateShader(GL_VERTEX_SHADER, vertexSource);
    // Создание фрагментного шейдера
    unsigned int fragmentShaderId = CreateShader(GL_FRAGMENT_SHADER, fragmentSource);
    // Проверка, успешно ли созданы оба шейдера
    if (vertexShaderId != 0 && fragmentShaderId != 0) {
        // Создание программы шейдеров
        id = CreateProgram(vertexShaderId, fragmentShaderId);
    } else {
        // Вывод сообщения об ошибке создания шейдера
        fprintf(stderr, "Could not create shader program\n");
    }
}

// Деструктор класса Shader
Shader::~Shader() {
    // Удаление программы шейдеров
    glDeleteProgram(id);
}

// Метод для привязки шейдера
void Shader::Bind() const {
    // Использование программы шейдеров
    glUseProgram(id);
}

// Метод для отвязывания шейдера
void Shader::Unbind() const {
    // Отключение программы шейдеров
    glUseProgram(0);
}

// Метод для создания шейдера
unsigned int Shader::CreateShader(unsigned int shaderType, const std::string &shaderSource) {
    // Создание шейдера указанного типа
    unsigned int shaderId = glCreateShader(shaderType);
    // Установка исходного кода шейдера
    SetShaderSource(shaderId, shaderSource);
    // Компиляция шейдера
    bool compiled = CompileShader(shaderId);
    // Проверка успешности компиляции
    if (compiled) {
        return shaderId; // Возвращение идентификатора шейдера
    } else {
        // Удаление шейдера в случае ошибки компиляции
        glDeleteShader(shaderId);
        return 0; // Возвращение 0 в случае ошибки
    }
}

// Метод для установки исходного кода шейдера
void Shader::SetShaderSource(unsigned int shaderId, const std::string &shaderSource) {
    // Преобразование строки в формат, необходимый для OpenGL
    char *source = (char *) shaderSource.c_str();
    unsigned int count = 1; // Количество шейдеров
    int *length = nullptr; // Длина исходного кода (nullptr для автоматического определения)
    // Установка исходного кода шейдера
    glShaderSource(shaderId, count, &source, length);
}

// Метод для компиляции шейдера
bool Shader::CompileShader(unsigned int shaderId) {
    // Компиляция шейдера
    glCompileShader(shaderId);
    int status = 0; // Переменная для хранения статуса компиляции
    // Получение статуса компиляции
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
    bool compiled = status != 0; // Проверка успешности компиляции
    // Вывод сообщения об ошибке в случае неудачи
    if (!compiled) {
        fprintf(stderr, "Could not compile shader\n");
    }
    return compiled; // Возвращение статуса компиляции
}

// Метод для создания программы шейдеров
unsigned int Shader::CreateProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId) {
    // Создание программы
    unsigned int programId = glCreateProgram();
    // Присоединение вершинного шейдера
    glAttachShader(programId, vertexShaderId);
    // Присоединение фрагментного шейдера
    glAttachShader(programId, fragmentShaderId);
    // Линковка программы шейдеров
    bool linked = LinkProgram(programId);
    // Проверка успешности линковки
    if (linked) {
        // Отключение присоединенных шейдеров
        glDetachShader(programId, vertexShaderId);
        glDetachShader(programId, fragmentShaderId);
        return programId; // Возвращение идентификатора программы
    } else {
        // Удаление программы в случае неудачи
        glDeleteProgram(programId);
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
        return 0; // Возвращ
    }
}

bool Shader::LinkProgram(unsigned int programId) {
    glLinkProgram(programId);
    int status = 0;
    glGetProgramiv(programId, GL_LINK_STATUS, (int *) &status);
    bool linked = status != 0;
    if (!linked) {
        fprintf(stderr, "Could not link shader program\n");
    }
    return linked;
}