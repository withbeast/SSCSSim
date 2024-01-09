//
// Created by 15838 on 2023/3/6.
//
#pragma once
#include "macro.h"

#ifndef SIMPLECPUSIM_MODEL_H
#define SIMPLECPUSIM_MODEL_H
struct Population{
    int id;
    int num;
    bool isSource;
    NeuronType type;
    std::vector<int> neurons;
};
struct Projection{
    int src;
    int tar;
    std::array<real,2> wrange;
    std::array<real,2> drange;
    float type;
    SynapseType stype;

};

class Model {
public:
    std::vector<Population*> pops;
    std::vector<Projection*> pros;
    int indexer;
    Model(){
        indexer=0;
    }
    Population& createPop(int num,NeuronType type,bool isSource=false){
        Population *p=new Population();
        p->num=num;
        p->id=indexer++;
        p->isSource=isSource;
        p->type=type;
        pops.push_back(p);
        return *p;
    }
    bool connect(Population& src, Population tar, std::array<real,2> _wrange, std::array<real,2> _drange, float type){
        connect(src,tar,_wrange,_drange,type,SynapseType::Normal);
    }
    bool connect(Population& src, Population tar, std::array<real,2> _wrange, std::array<real,2> _drange, float type,SynapseType stype){
        Projection* p=new Projection();
        p->src=src.id;
        p->tar=tar.id;
        p->wrange=_wrange;
        p->drange=_drange;
        p->type=type;
        p->stype=stype;
        if(type==0.0&&src.num!=tar.num)return false;
        int index=-1;
        for(int i=0;i<pros.size();i++){
            if(pros[i]->src==src.id&&(pros[i]->tar)==tar.id){
                index=i;
            }
        }
        if(index>0){//覆盖
            pros[index]->wrange=_wrange;
            pros[index]->drange=_drange;
            pros[index]->type=type;
        }else{
            pros.push_back(p);
        }
        return true;
    }

};


#endif //SIMPLECPUSIM_MODEL_H
