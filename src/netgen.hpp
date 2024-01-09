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

    static real randweight(real floor,real ceil){
        if(ceil==floor)return floor;
        
        int range=(int)((ceil-floor)/Config::DW) + 1;
        real tw;
        if(floor<0)
            tw=floor+(rand()%range)*Config::DW;
        else
            tw=floor-(rand()%range)*Config::DW;
        return tw;

    }
    static real randdelay(real floor,real ceil){
        if(ceil==floor)return floor;
        int range=(int)((ceil-floor)/Config::DT) + 1;
        return floor+(rand()%range)*Config::DT;
    }
    static Network& genNet(Model* model){
        auto *net=new Network(model);
        for(auto pop : model->pops){
            int pid=pop->id;
            int num=pop->num;
            net->pushPop(pop);
            for(int j=0;j<num;j++){
                int id=net->pushNeuron(pop->type,pop->isSource);
                pop->neurons.push_back(id);
            }
        }
        for(int i=0;i<model->pros.size();i++){
            int src=model->pros[i]->src;
            int tar=model->pros[i]->tar;
            int sn=model->pops[src]->num;
            int tn=model->pops[tar]->num;
            SynapseType stype=model->pros[i]->stype;
            real minw=model->pros[i]->wrange[0];
            real maxw=model->pros[i]->wrange[1];
            real mind=model->pros[i]->drange[0];
            real maxd=model->pros[i]->drange[1];
            if(model->pros[i]->type==1.0){
                for(int m=0;m<sn;m++){
                    for(int n=0;n<tn;n++){
                        net->pushSynapse(stype,model->pops[src]->neurons[m],model->pops[tar]->neurons[n],randweight(minw,maxw),randdelay(mind,maxd));
                    }
                }
            }
            else if(model->pros[i]->type==0.0){
                for(int k=0;k<sn;k++){
                    net->pushSynapse(stype,model->pops[src]->neurons[k],model->pops[tar]->neurons[k],randweight(minw,maxw),randdelay(mind,maxd));
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
                            net->pushSynapse(stype,model->pops[src]->neurons[m],model->pops[tar]->neurons[n],randweight(minw,maxw),randdelay(mind,maxd));
                    }
                }
            }
        }
        return *net;
    }
};


#endif //SIMPLECPUSIM_NETGEN_HPP
