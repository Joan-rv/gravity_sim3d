#ifndef TIMER_HPP
#define TIMER_HPP

class Timer {
public:
    Timer();
    double tick();

private:
    double last_time_;
};

#endif
