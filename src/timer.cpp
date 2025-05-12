
#include <GLFW/glfw3.h>

#include "timer.hpp"

Timer::Timer() : last_time_(glfwGetTime()) {};

double Timer::tick() {
    double curr_time = glfwGetTime();
    double dt = curr_time - last_time_;
    last_time_ = curr_time;
    return dt;
};
