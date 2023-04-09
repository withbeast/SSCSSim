//
// Created by 15838 on 2023/3/7.
//
#pragma once
#ifndef SIMPLECPUSIM_MACRO_H
#define SIMPLECPUSIM_MACRO_H

#include <vector>
#include <queue>
#include <algorithm>
#include <math.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <time.h>
#include <stdio.h>
#include <array>

#include <random>
typedef float real;
enum NeuronType
{
    RAW,
    LIF,
    POISSON,
    LIF2
};

class SpikePipe
{
public:
    real *data;
    int index;
    int size;
    SpikePipe()
    {
        data = nullptr;
        index = -1;
        size = 0;
    }
    SpikePipe(int _size, real d)
    {
        initialize(_size, d);
    }
    ~SpikePipe()
    {
        index = 0;
        size = 0;
        delete data;
    }
    void initialize(int _size, real d)
    {
        size = _size;
        index = -1;
        data = new real[_size];
        std::fill_n(data, _size, d);
    };
    real push(real d)
    {
        index = (index + 1) % size;
        real pre = data[index];
        data[index] = d;
        return pre;
    }
    real front() const
    {
        int f = (index + 1) % size;
        return data[f];
    }
    real back() const
    {
        return data[index];
    }
};
class InputBuffer
{
    real *_buffer;
    real _defaultValue;
    int _size;

public:
    InputBuffer(int size, real value = 0)
    {
        _size = size;
        _defaultValue = value;
        _buffer = new real[_size];
        std::fill_n(_buffer, _size, _defaultValue);
    }
    ~InputBuffer()
    {
        _size = 0;
        delete[] _buffer;
    }
    int size()
    {
        return _size;
    }
    real &operator[](int index)
    {
        return _buffer[index];
    }
    void update(int i, real value)
    {
        _buffer[i] = value;
    }
    void clear()
    {
        std::fill_n(_buffer, _size, _defaultValue);
    }
};
struct input_spike
{
    real time;
    int nid;
    real in;
    input_spike(real _time, int _nid, real _in)
    {
        time = _time;
        nid = _nid;
        in = _in;
    }
};
class Config
{
public:
    static real STEP;
    static real DT;
    static real TIME;
    static real DW;
    static void setTimestep(real time_step)
    {
        STEP = time_step;
        DT=time_step;
    }
    static void setTimes(real times)
    {
        TIME = times;
    }
    static void setDW(real dw){
        DW=dw;
    }
    static int getSteps(){
        return round(TIME/STEP);
    }
};

real Config::STEP = 0.001;
real Config::TIME = 1;
real Config::DT=0.001;
real Config::DW=0.01;

#endif // SIMPLECPUSIM_MACRO_H
