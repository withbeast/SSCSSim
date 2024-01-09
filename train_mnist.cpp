

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
using namespace std;
std::vector<int> cur_spikes;
std::vector<vector<int>> spike_counts;

void get_new_assignments(vector<vector<int>>& result,vector<short>& labels,vector<short>& assignments){
    int row=result.size();
    int col=result[0].size();
    vector<real> max_rate(col,0.0);
    for(int category=0;category<10;category++){
        int cate_num=0;
        vector<int> cate_arr(col,0);
        for(int i=0;i<result.size();i++){
            if(labels[i]==category){
                cate_num++;
                for(int j=0;j<cate_arr.size();j++){
                    cate_arr[j]+=result[i][j];
                }
            }
        }
        if(cate_num>0){
            for(int i=0;i<col;i++){
                real rate=(real)cate_arr[i]/cate_num;
                if(rate>max_rate[i]){
                    max_rate[i]=rate;
                    assignments[i]=category;
                }
            }   
        }
    }
}

short get_recognized_number_ranking(vector<short>& assignments,vector<int>& cur_spike){
    int dimension=assignments.size();
    real max_rate=0.0;
    short max_rank=0;
    for(int category=0;category<10;category++){
        int cate_num=0;
        int cate_spike=0;
        for(int i=0;i<dimension;i++){
            if(assignments[i]==category){
                cate_num++;
                cate_spike+=cur_spike[i];
            }
        }
        real cate_rate=(double)cate_rate/cate_num;
        if(cate_rate>max_rate){
            max_rate=cate_rate;
            max_rank=category;
        }
    }
    return max_rank;
}

void get_slice(vector<short>& all,vector<short>& slice,int index,int slice_num){
    slice.resize(slice_num);
    for(int i=index,idx=0;i<(index+slice_num)&&i<all.size();i++){
        slice[idx++]=all[i];
    }
}


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

void start_sim(){
    Config::setDW(0.01);
    Config::setTimestep(0.001);
    Model model;
    Population& Xe=model.createPop(784,NeuronType::POISSON, true);
    Population& Ae=model.createPop(400,NeuronType::LIF);
    Population& Ai=model.createPop(400,NeuronType::LIF);
    model.connect(Xe,Ae,{1,1},{0.003,0.01},1,SynapseType::STDP);
    model.connect(Ae,Ai,{1,1},{0.003,0.01},0,SynapseType::STDP);
    model.connect(Ai,Ae,{-1,-1},{0.003,0.01},1,SynapseType::STDP);
    Network net=NetGen::genNet(&model);
    Simulator sim(&net);
    tid=Ae.id;
    int AeSize=400;
    cur_spikes.resize(AeSize);
    sim.setMonitorPop([](int clock,int popid,vector<Neuron*>ns){
        if(popid==tid){
            for(int i=0;i<ns.size();i++){
                cur_spikes[i]+=ns[i]->isFired();
            }
        }
    });
    vector<short> assignments(AeSize);
    real single_time=0.35;
    bool train=true;
    if(train){
        /**
        * 训练网络
        */
        vector<vector<real>> train_images;
        read_images_poisson("../mnist/train-images.idx3-ubyte",train_images);
        vector<short> train_labels;
        read_Mnist_Label("../mnist/train-labels.idx1-ubyte",train_labels);
        
        int train_num=2;
        int train_slice=2;
        int bench_size=train_num/train_slice;
        spike_counts=vector<vector<int>>(train_slice);
        for(int bench=0;bench<bench_size;bench++){
            for(int i=0;i<train_slice;i++){
                int idx=bench*train_slice+i;
                sim.setPoissonData(train_images[idx]);
                sim.simulate(single_time);
                spike_counts[i]=vector<int>(cur_spikes);
                cur_spikes=vector<int>(400,0);
                cout<<"===== train "<<"image : "<<idx<<" ======"<<endl;
            }
            vector<short> labels_slice;
            get_slice(train_labels,labels_slice,bench*train_slice,train_slice);
            get_new_assignments(spike_counts,labels_slice,assignments);
            spike_counts=vector<vector<int>>(train_slice);
        }
        
    }
    
    bool test=true;
    if(test){
        /**
         * 测试网络
        */
        vector<vector<real>> test_images;
        read_images_poisson("../mnist/t10k-images.idx3-ubyte",test_images);
        vector<short> test_labels;
        read_Mnist_Label("../mnist/t10k-labels.idx1-ubyte",test_labels);
        int test_num=2;
        int right=0;
        for(int i=0;i<test_num;i++){
            sim.setPoissonData(test_images[i]);
            sim.simulate(single_time);
            short label=get_recognized_number_ranking(assignments,cur_spikes);
            if(label==test_labels[i]){
                right++;
            }
            cur_spikes=vector<int>(400,0);
            cout<<"===== test "<<"image : "<<i<<" ======"<<endl;
        }
        real rate=(double)right/test_num;
        std::cout<<"rate: "<<rate<<std::endl;
    }
}
int main() {
    start_sim();

    
    return 0;
}
