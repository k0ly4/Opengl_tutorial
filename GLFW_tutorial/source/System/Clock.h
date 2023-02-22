#ifndef CLOCK_H
#define CLOCK_H
#include "System/Exception.h"
#include "Game/Lib/GLFW.h"

class sClock {
public:
    inline static double Time() noexcept{   return  glfwGetTime(); }
    inline static float fTime() noexcept{   return  static_cast<float>(glfwGetTime()); }
private:
};

class LogClock{
public:
    LogClock() { last_ = sClock::Time();}
    inline void log_sec(const char* info) noexcept {
        LOG("%s:%f sec\n",info, sClock::fTime() - last_);
        last_ = sClock::fTime();
    }
    inline void log(const char* info) noexcept {
        LOG("%s:%f ms\n", info, (sClock::fTime() - last_)*1000.f);
        last_ = sClock::fTime();
    }
    inline void logIf_sec(float min, const char* info) noexcept {
        float tmp = sClock::fTime() - last_;
        if (tmp > min) {
            LOG("%s:%f sec\n", info, tmp);
            last_ = sClock::fTime();
        }
    }
 

private:
    float last_;
};

class Clock {

public:

    Clock() {
        lastTime_ = sClock::Time();
        deltaTime_ = 0.f;
    }
    inline double restart() {
        deltaTime_ = sClock::Time() - lastTime_;
        lastTime_ =  sClock::Time();
        return deltaTime_;
    }
    inline double delta() { return deltaTime_; }
    inline double total() { return lastTime_;}

private:
    double lastTime_, deltaTime_;
};

#endif


