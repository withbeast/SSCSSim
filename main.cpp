

#include "src/macro.h"
#include "src/model.hpp"
#include "src/netgen.hpp"
#include "src/simulator.hpp"
#include "src/network.hpp"
#include "src/util.hpp"
#include "src/spikegen.hpp"

std::vector<real> outputsu;
std::vector<real> outputsi;
std::vector<real> outputso;
std::vector<std::vector<bool>> inputPoisson;
void startSim(){
    model model;
    Population& p1=model.createPop(4, true);
    Population& p2=model.createPop(4);
    Population& p3=model.createPop(4);
    model.connect(p1,p2,1,0.002,1);
    model.connect(p1,p3,0.8,0.003,1);
    model.connect(p2,p3,1,0.003,1);
    NetGen netgen;
    Network net=netgen.genNet(&model);
    SpikeGen sgen;
    Simulator sim(&net,0.001);
    inputPoisson=sgen.randGenPoisson(200,4);

    sim.setFeeder([](int step,InputBuffer& buffer){
        for(int i=0;i<buffer.size();i++){
            buffer[i]=inputPoisson[step][i]?1.5:0;
        }
    });
    sim.setMonitor([](int id,real u,real i,real o){
        if(id==0){
            outputsu.push_back(u);
            outputsi.push_back(i);
            outputso.push_back(o);
        }
    });

    sim.simulate(0.2f);
    store(outputsu,"../u.txt");
    store(outputsi,"../i.txt");
    store(outputso,"../o.txt");

}

void update(real& U){
    U=U+10;
}

int main() {
    startSim();
    return 0;
}
