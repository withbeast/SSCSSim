

#include "src/macro.h"
#include "src/model.hpp"
#include "src/netgen.hpp"
#include "src/simulator.hpp"
#include "src/network.hpp"
#include "src/util.hpp"
#include "src/spikegen.hpp"
#include "src/dataloader.hpp"
std::vector<real> outputsu;
std::vector<real> outputsi;
std::vector<real> outputso;
std::vector<real> poissono;
std::vector<std::vector<bool>> inputPoisson;
void startSim(){
    Config::setTimestep(1);
    Model model;
    Population& p1=model.createPop(4,NeuronType::POISSON, true);
    Population& p2=model.createPop(4,NeuronType::LIF2);
    Population& p3=model.createPop(4,NeuronType::LIF2);
    model.connect(p1,p2,{1,1},{0.002,0.002},TYPE_FULL);
    model.connect(p1,p3,{1,1},{0.003,0.003},TYPE_FULL);
    model.connect(p2,p3,{1,1},{0.003,0.003},TYPE_FULL);
    NetGen netgen;
    Network net=netgen.genNet(&model);
    SpikeGen sgen;
    Simulator sim(&net);
    vector<real> randData=sgen.genRandPoissonRateData(4);
    sim.setPoissonData(randData);
    sim.setMonitorNeuron([](int step,Neuron& n){
        Neuron& ln= *dynamic_cast<Neuron*>(&n);
        if(n.getId()==0){
            poissono.push_back(ln.isFired());
        }
        if(n.getId()==5){
            outputsu.push_back(ln.getvm());
            outputsi.push_back(ln.getin());
            outputso.push_back(ln.isFired());
        }
    });
    sim.setMonitorSynapse([](int clock,Synapse& s){
        // STDPSynapse& ss=*dynamic_cast<STDPSynapse*>(&s);
        // cout<<"ss:"<<s.weight<<endl;
        // if(ss.src==0&&ss.tar==5){
            // std::cout<<clock<<":"<<ss.weight<<std::endl;
        // }
    });

    sim.simulate(200.0f);
    store(outputsu,"../analysis/u.txt");
    store(outputsi,"../analysis/i.txt");
    store(outputso,"../analysis/o.txt");
    store(poissono,"../analysis/po.txt");
    std::cout<<"simulate complete"<<std::endl;
}


int main() {
    // LIF2Constants* Consts = new LIF2Constants();
    // Consts->init();
    startSim();
    // vector<vector<real>> images;
    // read_images_poisson("../mnist/train-images.idx3-ubyte",images);
    // cout.width(2);
    // for(int i=0;i<28;i++){
    //     for(int j=0;j<28;j++){
    //         cout<<images[0][i*28+j]<<"\t";
    //     }
    //     cout<<endl;
    // }
    return 0;
}
