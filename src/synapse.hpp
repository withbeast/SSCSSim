#pragma once
#include "macro.h"
#include "neuron.hpp"
class Synapse
{
    public:
    int src;
    int tar;
    real weight;
    real delay;
    SpikePipe spikes;
    public: 
    virtual real update(Neuron& pre,Neuron& post){
        real i=weight*pre.isFired();
        return spikes.push(i)*weight;
    }

};
class STDPSynapse:public Synapse{
private:
    real A_LTP=0.1;
    real A_LTD=-0.01;
    real TAU_LTP=17;
    real TAU_LTD=34;
    real W_max=40;
    real W_min=0;
public:
    virtual real update(Neuron& pre,Neuron& post){
        
        real o=Synapse::update(pre,post);
        ///STDP
        RecordNeuron& pre_r=*dynamic_cast<RecordNeuron*>(&pre);
        RecordNeuron& post_r=*dynamic_cast<RecordNeuron*>(&post);
        if(pre.isFired()||post.isFired()){
            int dt=pre_r.getLastFired()-post_r.getLastFired();
            real dw=0;
            if(dt<0){
                dw=A_LTP*exp(dt/TAU_LTP);
            }else if(dt>0){
                dw=A_LTD*exp(-dt/TAU_LTD);
            }else{
                dw=0;
            }
            weight+=dw;
            weight=(weight>W_max)?W_max:((weight<W_min)?W_min:weight);
        }
        return o;
    }
};