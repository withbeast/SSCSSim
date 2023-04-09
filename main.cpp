

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
std::vector<std::vector<bool>> inputPoisson;
void startSim(){
    Config::setTimestep(0.001);
    Model model;
    Population& p1=model.createPop(4,NeuronType::LIF, true);
    Population& p2=model.createPop(4,NeuronType::LIF);
    Population& p3=model.createPop(4,NeuronType::LIF);
    model.connect(p1,p2,1,0.002,1);
    model.connect(p1,p3,1.0,0.003,1);
    model.connect(p2,p3,1,0.003,1);
    NetGen netgen;
    Network net=netgen.genNet(&model);
    SpikeGen sgen;
    Simulator sim(&net);
    vector<real> randData=sgen.genRandPoissonRateData(4);
    sim.setPoissonData(randData);
    sim.setMonitorNeuron([](int step,Neuron& n){
        LIFNeuron& ln= *dynamic_cast<LIFNeuron*>(&n);
        if(n.getId()==0){
            outputsu.push_back(ln.getV());
            outputsi.push_back(ln.getIn());
            outputso.push_back(ln.isFired());
        }
    });
    sim.setMonitorSynapse([](int clock,Synapse& s){
        STDPSynapse& ss=*dynamic_cast<STDPSynapse*>(&s);
        // cout<<"ss:"<<s.weight<<endl;
        if(ss.src==0&&ss.tar==5){
            std::cout<<clock<<":"<<ss.weight<<std::endl;
        }
    });

    sim.simulate(0.2f);
    store(outputsu,"../u.txt");
    store(outputsi,"../i.txt");
    store(outputso,"../o.txt");
    std::cout<<"simulate complete"<<std::endl;
}


int main() {
    // startSim();
    vector<vector<real>> images;
    read_images_poisson("../mnist/train-images.idx3-ubyte",images);
    cout.width(2);
    for(int i=0;i<28;i++){
        for(int j=0;j<28;j++){
            cout<<images[0][i*28+j]<<"\t";
        }
        cout<<endl;
    }
    return 0;
}
