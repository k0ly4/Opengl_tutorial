#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
class LibInit {
    bool INIT_LIB() {
        std::cout << "initLib\n";
        if (!glfwInit())
        {
            std::cout << "(!)Failed to Initialization GLFW lib" << std::endl;
            return 0;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //  glfwWindowHint(GLFW_SAMPLES, 4);
        window = glfwCreateWindow(200, 200, "Window", NULL, NULL);
        //Контекст и дебаг
        glfwMakeContextCurrent(window);
        glfwSetErrorCallback(error_callback);
        
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return 0;
        }
        // glad: загрузка всех указателей на OpenGL-функции
        return 1;
    }
    GLFWwindow* window;
public:
    GLFWwindow* getContext() {
        return window;
    }
    LibInit() { INIT_LIB(); }
}GL_LIB;

