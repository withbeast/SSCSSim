//
// Created by 15838 on 2023/3/9.
//

#ifndef SIMPLECPUSIM_UTIL_HPP
#define SIMPLECPUSIM_UTIL_HPP
#include "macro.h"
void store(std::vector<real> arr,std::string filename){
    std::ofstream file;
    file.open(filename);
    if (!file.is_open()) {
        std::cout << "failed to open file" << filename << std::endl;
        return;
    }
    for(int i=0;i<arr.size();i++){
        if(i==arr.size()-1)
            file << arr[i]<<"";
        else
            file<<arr[i]<<",";
    }
    file.close();
}
void store(std::vector<int> arr,std::string filename){
    std::ofstream file;
    file.open(filename);
    if (!file.is_open()) {
        std::cout << "failed to open file" << filename << std::endl;
        return;
    }
    for(int i=0;i<arr.size();i++){
        if(i==arr.size()-1)
            file << arr[i]<<"";
        else
            file<<arr[i]<<",";
    }
    file.close();
}
#endif //SIMPLECPUSIM_UTIL_HPP
