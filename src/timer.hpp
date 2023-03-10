//
// Created by 15838 on 2023/3/9.
//

#ifndef SIMPLECPUSIM_TIMER_HPP
#define SIMPLECPUSIM_TIMER_HPP
#include "macro.h"

class Timer {
public:
    Timer(const char *funcName) {
        _funcname = funcName;
        _begin = clock();
    }
    ~Timer() {
        _end = clock();
        _elapsed = _end - _begin;
        printf("Function name: %s\nElapsed : %f\n", _funcname, _elapsed);
    }
private:
    double _begin;
    double _end;
    double _elapsed;
    const char *_funcname;
};
#endif //SIMPLECPUSIM_TIMER_HPP
