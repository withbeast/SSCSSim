//
// Created by 15838 on 2023/3/6.
//
#pragma once
#ifndef SIMPLECPUSIM_NETWORK_HPP
#define SIMPLECPUSIM_NETWORK_HPP
#include "macro.h"


struct Neuron{
    bool isSource;
    int id;
    real v;
    real in;
    real out;
};
struct Synapse{
    int src;
    int tar;
    real weight;
    real delay;
    SpikePipe spikes;
};

class Network {
public:
    std::vector<Neuron*> source;
    std::vector<Neuron*> neurons;
    std::vector<Synapse*> synapses;
    int indexer=0;
    Network(){}
    ~Network(){
        for(auto & neuron : neurons){
            delete neuron;
        }
        neurons.clear();
        source.clear();
        for(auto & synapse : synapses){
            delete synapse;
        }
        synapses.clear();
    }
    int pushNeuron(bool isSource=false){
        auto* n=new Neuron();
        n->id=indexer++;
        n->isSource=isSource;
        neurons.push_back(n);
        if(isSource)source.push_back(n);
        return n->id;
    }
    void pushSynapse(int src,int tar,real weight,real delay){
        auto* syn=new Synapse();
        syn->tar=tar;
        syn->src=src;
        syn->weight=weight;
        syn->delay=delay;
        synapses.push_back(syn);
    }
};


#endif //SIMPLECPUSIM_NETWORK_HPP
