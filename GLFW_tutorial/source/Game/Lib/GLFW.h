#ifndef GLFW_H
#define GLFW_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <thread>
//nvidia descriptor
//extern "C" {
//    _declspec(dllexport) int NvOptimusEnablement = 0x00000001;
//}
//

#define MAX_THREADS 4
class GLFW {
   
public:

    class Context {

    public:

        GLFWwindow* get();
        Context() { initialize(); }
        void setActive(bool enable);

    private:
        bool lastStateThread = 0;
        std::thread::id lastThread;

        bool initialize();
        GLFWwindow* window;
        
    };

    static GLFWwindow* getContext() { return lib.get();}
    static inline void setActive(bool enable) { lib.setActive(enable);}
private:
    
    static Context lib;
};

#endif
