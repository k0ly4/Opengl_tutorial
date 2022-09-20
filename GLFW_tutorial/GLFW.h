#ifndef GLFW_H
#define GLFW_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
//nvidia descriptor
//extern "C" {
//    _declspec(dllexport) int NvOptimusEnablement = 0x00000001;
//}
//
class GLFW {
   
public:
    GLFWwindow* getContext();
    GLFW() { initialize(); }
private:
    bool initialize();
    GLFWwindow* window;
};


#endif
