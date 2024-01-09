//
// Created by 15838 on 2023/3/6.
//

#ifndef SIMPLECPUSIM_SIMULATOR_HPP
#define SIMPLECPUSIM_SIMULATOR_HPP

#include "macro.h"
#include "network.hpp"



class Simulator
{
public:
    Network *net;
    std::vector<real> output;
    real dt;
    void (*monitorn)(int, Neuron&);
    void (*monitors)(int,Synapse&);
    void (*monitorp)(int,int,std::vector<Neuron*>);

    Simulator(Network *_net)
    {
        net = _net;
        dt=Config::STEP;
        // 初始化突触脉冲缓冲池
        for (auto &synapse : net->synapses)
        {
            int size = std::lrint(synapse->delay / dt);
            synapse->spikes.initialize(size, 0);
        }
        
        monitorn = [](int clock, Neuron&n) {};
        monitors = [](int clock,Synapse&s) {};
        monitorp = [](int clock,int pop,std::vector<Neuron*> ns){};
    }
    ~Simulator()
    {
        output.clear();
    }
    void setPoissonData(std::vector<real> data)
    {
        
        Population* tp=nullptr;
        for(auto &p:net->pops){
            if(p->isSource){
                tp=p;
            }
        }
        if(tp==nullptr)return;
        for(int i=0;i<tp->neurons.size();i++){
            (*net)[tp->neurons[i]].recv(data[i]);
        }
    }
    void setFeeder(void (*_feeder)(int, InputBuffer &))
    {
        // feeder = _feeder;
    }
    void setMonitorNeuron(void (*_monitor)(int, Neuron&))
    {
        monitorn = _monitor;
    }
    void setMonitorSynapse(void (*_monitors)(int,Synapse&)){
        monitors=_monitors;
    }
    void setMonitorPop(void (*_monitorp)(int,int,std::vector<Neuron*>)){
        monitorp=_monitorp;
    }
    void simulate(real time)
    {
        int steps = std::lrint(time / dt);
        for (int i = 0; i < steps; i++)
        {
            // feeder(i, *inputs);
            simulate(i);
            // inputs->clear();
        }
    }
private:
    void simulate(int clock) 
    {
        // // 准备输入脉冲
        // for (int i = 0; i < inputs->size(); i++)
        // {
        //     net->source[i]->recv((*inputs)[i]);
        // }
        // 仿真神经元
        for(auto p:net->pops){
            std::vector<Neuron*> ns;
            for(auto i:p->neurons){
                //更新神经元
                (*net)[i].update(clock);
                ns.push_back((*net).neurons[i]);
                monitorn(clock,(*net)[i]);
            }
            monitorp(clock,p->id,ns);
            for(auto i:p->neurons){
                //清楚神经元的输入电流
                (*net)[i].clear();
            }
        }
        // 仿真突触
        for (auto synapse : net->synapses)
        {
            real o=synapse->update(*(net->neurons[synapse->src]),*(net->neurons[synapse->tar]));
            monitors(clock,*synapse);
            net->neurons[synapse->tar]->recv(o);
        }
    }
};

#endif // SIMPLECPUSIM_SIMULATOR_HPP
