//
// Created by 15838 on 2023/3/6.
//

#ifndef SIMPLECPUSIM_SIMULATOR_HPP
#define SIMPLECPUSIM_SIMULATOR_HPP

#include "macro.h"
#include "network.hpp"

const real R = 5.1;
const real C = 5e-3;
const real thresh = 0.5;


void LIF(real& U,const real& I,real& out,real step){
    real tau = R * C;
    out=(U>thresh);
    U = U + (step / tau) * (I * R - U) - out*thresh;
}

class Simulator {
public:
    Network *net;
    InputBuffer* inputs;
    std::vector<real> output;
    real dt;
    void (*monitor)(int,real,real,real);
    void (*feeder)(int, InputBuffer&);

    Simulator(Network* _net,real _dt){
        net=_net;
        dt=_dt;
        monitor=[](int id,real u,real i,real o){};
        feeder=[](int step, InputBuffer& buffer){};
        inputs=new InputBuffer(net->source.size());
    }
    ~Simulator(){
        delete inputs;
        output.clear();
    }
    void setData(std::vector<InputBuffer> data){

    }
    void setFeeder(void (*_feeder)(int,InputBuffer&)){
        feeder=_feeder;
    }
    void setMonitor(void (*_monitor)(int,real,real,real)){
        monitor=_monitor;
    }
    void simulate(real time) {
        //初始化突触脉冲缓冲池
        for(auto & synapse : net->synapses){
            int size=std::lrint(synapse->delay/dt);
            synapse->spikes.initialize(size,0);
        }
        int steps = std::lrint(time / dt);
        for (int i = 0; i < steps; i++) {
            feeder(i, *inputs);
            simulate(i);
            inputs->clear();
        }
    }

    void simulate(int step) {
        //准备输入脉冲
        for(int i=0;i<inputs->size(); i++){
            net->source[i]->in=(*inputs)[i];
        }
        //仿真神经元
        for (auto & neuron : net->neurons) {
            LIF(neuron->v, neuron->in, neuron->out, dt);
            monitor(neuron->id,neuron->v,neuron->in,neuron->out);
            neuron->in=0;
        }
        //仿真突触
        for(auto & synapse : net->synapses){
            int src=synapse->src;
            int tar=synapse->tar;
            real i=net->neurons[src]->out*synapse->weight;
            real o=synapse->spikes.push(i);
            net->neurons[tar]->in+=o;
        }
        std::cout<<net->neurons[4]->in<<std::endl;
    }

};


#endif //SIMPLECPUSIM_SIMULATOR_HPP
