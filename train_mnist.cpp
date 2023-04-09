

#include "src/macro.h"
#include "src/model.hpp"
#include "src/netgen.hpp"
#include "src/simulator.hpp"
#include "src/network.hpp"
#include "src/util.hpp"
#include "src/spikegen.hpp"
#include "src/dataloader.hpp"
// std::vector<real> outputsu;
// std::vector<real> outputsi;
// std::vector<real> outputso;
// std::vector<std::vector<bool>> inputPoisson;
int tid=0;
std::vector<int> cur_spikes;
std::vector<vector<int>> spike_counts;

void print_image(vector<int>& image){
    for(int i=0;i<28;i++){
        for(int j=0;j<28;j++){
            cout<<image[i*28+j]<<"\t";
        }
        cout<<endl;
    }
    cout<<endl;
}
void print_label(vector<short>& label){
    for(int i=0;i<label.size();i++){
        cout<<label[i]<<"\t";
    }
    cout<<endl;
}

void startSim(){
    Config::setDW(0.01);
    Config::setTimestep(0.001);
    Config::setTimes(0.1);
    Model model;
    Population& Xe=model.createPop(784,NeuronType::POISSON, true);
    Population& Ae=model.createPop(400,NeuronType::LIF);
    Population& Ai=model.createPop(400,NeuronType::LIF);
    model.connect(Xe,Ae,{1,1},{0.003,0.01},1);
    model.connect(Ae,Ai,{1,1},{0.003,0.01},0);
    model.connect(Ai,Ae,{-1,-1},{0.003,0.01},1);
    Network net=NetGen::genNet(&model);
    Simulator sim(&net);
    vector<vector<real>> train_images;
    read_images_poisson("../mnist/train-images.idx3-ubyte",train_images);
    // vector<short> train_labels;
    // read_Mnist_Label("../mnist/train-labels.idx1-ubyte",train_labels);
    // vector<vector<real>> test_images;
    // read_images_poisson("../mnist/t10k-images.idx3-ubyte",test_images);
    // vector<short> test_labels;
    // read_Mnist_Label("../mnist/t10k-labels.idx1-ubyte",test_labels);
    // vector<real> onepic;
    // read_onepic_poisson("../mnist/train-images.idx3-ubyte",onepic);
    tid=Ae.id;
    cur_spikes=vector<int>(400,0);
    sim.setMonitorPop([](int clock,int popid,vector<Neuron*>ns){
        if(popid==tid){
            for(int i=0;i<ns.size();i++){
                cur_spikes[i]+=ns[i]->isFired();
            }
        }
    });
    cout<<"start simulate"<<endl;
    for(int i=0;i<100;i++){
        cout<<"====="<<"image:"<<i<<"======"<<endl;
        sim.setPoissonData(train_images[i]);
        sim.simulate(Config::TIME);
    }
    
    // for(int i=0;i<cur_spikes.size();i++){
    //     cout<<cur_spikes[i]<<"\t";
    // }
    // cout<<endl;


    std::cout<<"simulate complete"<<std::endl;
}
int main() {
    startSim();
    return 0;
}
