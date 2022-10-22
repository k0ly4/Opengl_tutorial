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

    class Context {

    public:

        GLFWwindow* get();
        Context() { initialize(); }

    private:

        bool initialize();
        GLFWwindow* window;

    };

 static GLFWwindow* getContext() {
        return lib.get();
    }

private:
    
    static Context lib;
};

#endif
