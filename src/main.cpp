#include <iostream> // Подключение библиотеки для работы с вводом и выводом
#include <glad/glad.h> // Подключение библиотеки GLAD для загрузки указателей на функции OpenGL
#include <GLFW/glfw3.h> // Подключение библиотеки GLFW для работы с окнами и управления контекстом OpenGL
#include "Shader.h" // Подключение пользовательского класса Shader для работы с шейдерами

// Определение констант ширины и высоты окна
const unsigned int WIDTH = 800; 
const unsigned int HEIGHT = 600;

// Определение количества вершин и размера каждой вершины
const unsigned int NUMBER_OF_VERTICES = 3; 
const unsigned int VERTEX_SIZE = 2; 
// Определение координат вершин треугольника
const float VERTICES[NUMBER_OF_VERTICES * VERTEX_SIZE] = {
        -0.5f, -0.5f, // Нижняя левая вершина
        0.0f, 0.5f,  // Верхняя вершина
        0.5f, -0.5f  // Нижняя правая вершина
};

// Определение количества индексов треугольника
const unsigned int NUMBER_OF_INDICES = 3; 
// Определение индексов вершин для рисования треугольника
const unsigned int INDICES[NUMBER_OF_INDICES] = {
        0, 1, 2 // Индексы, указывающие порядок соединения вершин
};

// Функция для обработки нажатия клавиш
void onKeyPressed(GLFWwindow *window, int key, int scanCode, int action, int mode) {
    std::cout << "Key pressed [" << key << "]" << std::endl; // Вывод нажатой клавиши в консоль
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { // Проверка нажатия клавиши ESC
        std::cout << "Escape pressed. Window should close." << std::endl; // Сообщение о закрытии окна
        glfwSetWindowShouldClose(window, GL_TRUE); // Установка флага закрытия окна
    }
}

// Функция для проверки наличия ошибок OpenGL
bool isGLError() {
    while (GLenum error = glGetError()) { // Получение кода ошибки OpenGL
        std::cout << "GL error [" << error << "]" << std::endl; // Вывод ошибки в консоль
        return true; // Ошибка обнаружена
    }
    return false; // Ошибок нет
}

// Главная функция
int main() {
    std::cout << "Initializing GLFW" << std::endl; // Инициализация GLFW
    if (!glfwInit()) { // Проверка успешной инициализации
        fprintf(stderr, "Could not initialize GLFW\n"); // Сообщение об ошибке
        return -1; // Выход с ошибкой
    }

    std::cout << "Setting GLFW window hints" << std::endl; // Установка параметров окна GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Установка главной версии OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); // Установка минорной версии OpenGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Установка профиля OpenGL
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Установка совместимости
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Установка неподвижного окна

    std::cout << "Creating GLFW window" << std::endl; // Создание окна GLFW
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Untitled", nullptr, nullptr); // Инициализация окна
    if (window == nullptr) { // Проверка успешного создания окна
        fprintf(stderr, "Could not create GLFW window\n"); // Сообщение об ошибке
        glfwTerminate(); // Завершение работы GLFW
        return -1; // Выход с ошибкой
    }

    std::cout << "Configuring GLFW window" << std::endl; // Конфигурирование окна
    glfwMakeContextCurrent(window); // Установка текущего контекста окна
    glfwSetKeyCallback(window, onKeyPressed); // Установка функции обратного вызова для клавиатуры

    std::cout << "Initializing GLAD" << std::endl; // Инициализация GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) { // Проверка успешной загрузки GLAD
        fprintf(stderr, "Could not initialize GLAD\n"); // Сообщение

          return -1; // Возврат ошибки при неудачной инициализации GLAD
    }

    std::cout << "Graphics context:" << std::endl; // Вывод информации о графическом контексте
    std::cout << "-- Vendor " << glGetString(GL_VENDOR) << std::endl; // Вывод имени производителя графического устройства
    std::cout << "-- Renderer " << glGetString(GL_RENDERER) << std::endl; // Вывод информации о рендерере
    std::cout << "-- Version " << glGetString(GL_VERSION) << std::endl; // Вывод версии OpenGL

    unsigned int vertexBufferId; // Идентификатор буфера для вершин
    glGenBuffers(1, &vertexBufferId); // Генерация буфера
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId); // Привязка буфера вершин
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW); // Загрузка данных вершин в буфер

    unsigned int vertexArrayId; // Идентификатор массива вершин
    glGenVertexArrays(1, &vertexArrayId); // Генерация массива вершин
    glBindVertexArray(vertexArrayId); // Привязка массива вершин
    glEnableVertexAttribArray(0); // Активация атрибута вершин с индексом 0
    glVertexAttribPointer(0, VERTEX_SIZE, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), nullptr); // Установка указателя на данные атрибута вершин

    unsigned int indexBufferId; // Идентификатор буфера индексов
    glGenBuffers(1, &indexBufferId); // Генерация буфера индексов
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId); // Привязка буфера индексов
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW); // Загрузка данных индексов в буфер

    // Шейдерный код вершин
    std::string vertexSource = R"(
			#version 330 core // Указание версии GLSL

			layout(location = 0) in vec3 in_position; // Входные данные: позиции вершин

			// Выходная структура для шейдера вершин
			out Vertex {
				vec4 color; // Цвет вершины
			} out_vertex;

			void main()
			{
				out_vertex.color = vec4(1.0f, 1.0f, 1.0f, 1.0f); // Задание цвета вершины (белый)
				gl_Position = vec4(in_position, 1.0); // Задание позиции вершины в пространстве
			}
		)";

    // Шейдерный код фрагмента
    std::string fragmentSource = R"(
			#version 330 core // Указание версии GLSL

			in Vertex { // Входная структура из шейдера вершин
				vec4 color; // Цвет вершины
			} in_vertex;

			out vec4 out_color; // Выходной цвет фрагмента

			void main()
			{
				out_color = in_vertex.color; // Установка выходного цвета фрагмента
			}
		)";
        
    Shader shader(vertexSource, fragmentSource); // Создание шейдера с вершинным и фрагментным кодом
    shader.Bind(); // Привязка шейдера

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Установка цвета очистки (темно-синий)

    // Основной цикл рендеринга
    while (!glfwWindowShouldClose(window)) { // Проверка, не закрыто ли окно
        glfwPollEvents(); // Обработка событий

        glClear(GL_COLOR_BUFFER_BIT); // Очистка буфера цвета
        glDrawElements(GL_TRIANGLES, NUMBER_OF_INDICES, GL_UNSIGNED_INT, nullptr); // Отрисовка треугольника по индексам

        if (isGLError()) { // Проверка на наличие ошибок OpenGL
            fprintf(stderr, "Could not render\n"); // Сообщение об ошибке
            return -1; // Возврат ошибки
        }

        glfwSwapBuffers(window); // Переключение между передним и задним буфером
    }

    std::cout << "Destroying window" << std::endl; // Сообщение о разрушении окна
    glfwDestroyWindow(window); // Уничтожение окна
    std::cout << "Terminating GLFW" << std::endl;
    glfwTerminate();
    return 0;
}
