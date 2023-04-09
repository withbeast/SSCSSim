//
// Created by 15838 on 2023/3/6.
//
#pragma once
#ifndef SIMPLECPUSIM_NETWORK_HPP
#define SIMPLECPUSIM_NETWORK_HPP
#include "macro.h"
#include "model.hpp"
#include "neuron.hpp"
#include "synapse.hpp"

Neuron* createN(NeuronType type,int id,bool isSource){
    Neuron* n;
    switch (type)
    {
    case NeuronType::LIF:
        n=new LIFNeuron(id,isSource);
        /* code */
        break;
    case NeuronType::POISSON:
        n=new PoissonInNeuron(id);
        break;
    case NeuronType::LIF2:
        n=new LIFNeuron2(id,isSource);
        break;
    default:
        n=nullptr;
        break;
    }
    return n;
}

class Network
{
public:
    std::vector<Population* >pops;
    std::vector<Neuron *> neurons;
    std::vector<Synapse *> synapses;
    int indexer = 0;
    Network(Model* model) {
    }
    ~Network()
    {
        for (auto &neuron : neurons)
        {
            delete neuron;
        }
        neurons.clear();
        // source.clear();
        for (auto &synapse : synapses)
        {
            delete synapse;
        }
        synapses.clear();
    }
    void pushPop(Population* p){
        pops.push_back(p);
    };
    int pushNeuron(NeuronType type,bool isSource = false)
    {
        auto *n = createN(type,indexer++,isSource);
        neurons.push_back(n);
        return n->getId();
    }
    void pushSynapse(int src, int tar, real weight, real delay)
    {
        auto *syn = new STDPSynapse();
        syn->tar = tar;
        syn->src = src;
        syn->weight = weight;
        syn->delay = delay;
        synapses.push_back(syn);
    }
    Neuron& operator[](int index){
        return *neurons[index];
    }
    Neuron* get(int pop,int index){
        for(auto p:pops){
            if(p->id==pop){
                int offset=p->neurons[index];
                return neurons[offset];
            }
        }
        return nullptr;
    }
};
struct pop_data{
    int id;
    int num;
    bool source;
    short type;
};
struct neuron_data{
    int id;
    short type;
    bool source;
};
struct synapse_data{
    int src;
    int tar;
    real weight;
    real delay;
};

void save_network(string filename,Network& net){
    ofstream ofile(filename,std::ios::out|std::ios::binary);
    if(ofile.is_open()){
        //写族群
        int psize=net.pops.size();
        ofile.write((char*)&psize,sizeof(int));
        for(int i=0;i<psize;i++){
            Population* p=net.pops[i];
            pop_data pd={p->id,p->num,p->isSource,p->type};
            ofile.write((char*)&pd,sizeof(pop_data));
            int* narr=new int[p->num];
            for(int j=0;j<p->num;j++){
                narr[j]=p->neurons[j];
            }
            ofile.write((char*)&narr,sizeof(int)*p->num);
        }
        //写神经元
        int nsize=net.neurons.size();
        ofile.write((char*)&nsize,sizeof(int));
        for(int i=0;i<nsize;i++){
            Neuron* n=net.neurons[i];
            neuron_data nd={n->getId(),n->getType(),n->isSource()};
            ofile.write((char*)&nd,sizeof(neuron_data));
        }
        //写突触
        int ssize=net.synapses.size();
        ofile.write((char*)&ssize,sizeof(int));
        for(int i=0;i<ssize;i++){
            Synapse* s=net.synapses[i];
            synapse_data sd={s->src,s->tar,s->weight,s->delay};
            ofile.write((char*)&sd,sizeof(synapse_data));
        }
    }
    ofile.close();
};

void load_network(string filename,Network& net){};

#endif // SIMPLECPUSIM_NETWORK_HPP
