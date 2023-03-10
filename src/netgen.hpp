//
// Created by 15838 on 2023/3/6.
//
#pragma once
#ifndef SIMPLECPUSIM_NETGEN_HPP
#define SIMPLECPUSIM_NETGEN_HPP
#include "macro.h"
#include "model.hpp"
#include "network.hpp"

class NetGen {
public:
    NetGen(){}
    Network& genNet(model* model){
        auto *net=new Network();
        for(auto & pop : model->pops){
            int num=pop->num;
            for(int j=0;j<num;j++){
                int id=net->pushNeuron(pop->isSource);
                pop->neurons.push_back(id);
            }
        }
        for(int i=0;i<model->pros.size();i++){
            int src=model->pros[i]->src;
            int tar=model->pros[i]->tar;
            int sn=model->pops[src]->num;
            int tn=model->pops[tar]->num;
            real weight=model->pros[i]->weight;
            real delay=model->pros[i]->delay;
            if(model->pros[i]->type==1.0){
                for(int m=0;m<sn;m++){
                    for(int n=0;n<tn;n++){
                        net->pushSynapse(model->pops[src]->neurons[m],model->pops[tar]->neurons[n],weight,delay);
                    }
                }
            }
            else if(model->pros[i]->type==0.0){
                for(int k=0;k<sn;k++){
                    net->pushSynapse(model->pops[src]->neurons[k],model->pops[tar]->neurons[k],weight,delay);
                }
            }else if(model->pros[i]->type<1.0&&model->pros[i]->type>0.0){
                std::vector<bool> genlist(sn*tn);
                for(int k=0;k<genlist.size();k++){
                    if(k<std::round(genlist.size()*model->pros[i]->type)){
                        genlist[k]=true;
                    }else{
                        genlist[k]=false;
                    }
                }
                std::random_shuffle(genlist.begin(),genlist.end());
                for(int m=0;m<sn;m++){
                    for(int n=0;n<tn;n++){
                        if(genlist[m*tn+n])
                            net->pushSynapse(model->pops[src]->neurons[m],model->pops[tar]->neurons[n],weight,delay);
                    }
                }
            }
        }
        return *net;
    }
};


#endif //SIMPLECPUSIM_NETGEN_HPP
