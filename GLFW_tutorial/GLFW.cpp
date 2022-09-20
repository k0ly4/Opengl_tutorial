#include "GLFW.h"

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

bool GLFW::initialize() {
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
GLFWwindow* GLFW::getContext() {
        return window;
    }