#ifndef CLOCK_H
#define CLOCK_H
#include "Game/GLFW.h"
class Clock {

public:
    Clock() {
        lastTime_ = glfwGetTime();
        deltaTime_ = 0.f;
    }
    double restart() {
        deltaTime_ = glfwGetTime() - lastTime_;
        lastTime_ = glfwGetTime();
        return deltaTime_;
    }
    double total() {
        return lastTime_;
    }

private:
    double lastTime_, deltaTime_;
};
#endif


