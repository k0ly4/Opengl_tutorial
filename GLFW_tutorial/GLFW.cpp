#include "GLFW.h"
struct PreSetupContext {

    int width = 800;
    int height = 600;
    std::string name = "Window";
    GLFWmonitor* monitor = NULL;
    GLFWwindow* share = NULL;

};
void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

bool GLFW::Context::initialize() {
        if (!glfwInit())
        {
            printf("(!)Error::GLFW::Context::initialize()::Failed to init GLFW");
            exit(1);
            return 0;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //  glfwWindowHint(GLFW_SAMPLES, 4);
        PreSetupContext setup;
        window = glfwCreateWindow(setup.width, setup.height, setup.name.c_str(), setup.monitor, setup.share);
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
GLFWwindow* GLFW::Context::get() {
        return window;
    }

GLFW::Context GLFW::lib;
