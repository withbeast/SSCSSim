//
// Created by 15838 on 2023/3/6.
//
#pragma once
#ifndef SIMPLECPUSIM_SPIKEGEN_HPP
#define SIMPLECPUSIM_SPIKEGEN_HPP
#include "macro.h"
#include "network.hpp"
#include <random>
class SpikeGen {
public:
    SpikeGen(){}
    real random(){
        return rand() % (999 + 1) / (float)(999 + 1);
    }
    std::vector<input_spike> testGen(real step, int id, int start, int end, real exc){
        std::vector<input_spike> spikes;
        for(int i=0;i<start;i++){
            spikes.emplace_back(step*i,id,0);
        }
        for(int i=start;i<end;i++){
            spikes.emplace_back(step*i,id,exc);
        }
        return spikes;
    }
    std::vector<std::vector<bool>> randGenPoisson(int steps,int sourceNum){
        std::vector<real> data_init(sourceNum);
        std::vector<std::vector<bool>> data(steps,std::vector<bool>(sourceNum));
        for(int i=0;i<sourceNum;i++)
            data_init[i]=random();
        for(int i=0;i<steps;i++){
            for(int j=0;j<sourceNum;j++){
                if(random()<data_init[j]){
                    data[i][j]=true;
                }else{
                    data[i][j]=false;
                }
            }
        }
        return data;
    }
    std::vector<std::vector<bool>> mnistGenPoisson(int steps,std::vector<int>_image){
        int n=_image.size();
        std::vector<real> data_init(n);
        std::vector<std::vector<bool>> data(steps,std::vector<bool>(n));
        for(int i=0;i<n;i++)
            data_init[i]=(float)_image[i]/255.0f;
        for(int i=0;i<steps;i++){
            for(int j=0;j<n;j++){
                if(random()<data_init[j]){
                    data[i][j]=true;
                }else{
                    data[i][j]=false;
                }
            }
        }
        return data;
    }
};


#endif //SIMPLECPUSIM_SPIKEGEN_HPP
