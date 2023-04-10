//
// Created by 15838 on 2023/3/13.
//



#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "macro.h"
using namespace std;
/**
 * 读取MNIST数据集
 * 分为四个文件
 * train-images、train-lables、t10k-images、t10k-labels
 *
 * images文件的前四个int值为magic（魔数）、numbers（图片数量）、rows（图片的行数）、cols（图片的列数）
 * images文件的后续为unsigned byte流，按照行优先读取，每个byte值为像素点的灰度值（0-255）
 *
 * labels文件的前两个int值为magic（魔数）、numbers（标签数量）
 * labels文件的后续为unsigned byte流，每个byte表示对应图片的分类结果（0-9）
 */

int ReverseInt(int i) {
    unsigned char ch1, ch2, ch3, ch4;
    ch1 = i & 255;
    ch2 = (i >> 8) & 255;
    ch3 = (i >> 16) & 255;
    ch4 = (i >> 24) & 255;
    return ((int) ch1 << 24) + ((int) ch2 << 16) + ((int) ch3 << 8) + ch4;
}

void read_Mnist_Label(string filename, vector<short> &labels) {
    ifstream file(filename, ios::binary);
    if (file.is_open()) {
        int magic_number = 0;
        int number_of_images = 0;
        file.read((char *) &magic_number, sizeof(magic_number));
        file.read((char *) &number_of_images, sizeof(number_of_images));
        magic_number = ReverseInt(magic_number);
        number_of_images = ReverseInt(number_of_images);
        for (int i = 0; i < number_of_images; i++) {
            unsigned char label = 0;
            file.read((char *) &label, sizeof(label));
            labels.push_back((short) label);
        }

    }
}

void read_Mnist_Images(string filename, vector<vector<double>> &images) {
    ifstream file(filename, ios::binary);
    if (file.is_open()) {
        int magic_number = 0;
        int number_of_images = 0;
        int n_rows = 0;
        int n_cols = 0;
        unsigned char label;
        file.read((char *) &magic_number, sizeof(magic_number));
        file.read((char *) &number_of_images, sizeof(number_of_images));
        file.read((char *) &n_rows, sizeof(n_rows));
        file.read((char *) &n_cols, sizeof(n_cols));
        magic_number = ReverseInt(magic_number);
        number_of_images = ReverseInt(number_of_images);
        n_rows = ReverseInt(n_rows);
        n_cols = ReverseInt(n_cols);


        for (int i = 0; i < number_of_images; i++) {
            vector<double> tp;
            for (int r = 0; r < n_rows; r++) {
                for (int c = 0; c < n_cols; c++) {
                    unsigned char image = 0;
                    file.read((char *) &image, sizeof(image));
                    tp.push_back(image);
                }
            }
            images.push_back(tp);
        }
    }
}



void read_images_poisson(string filename,vector<vector<real>>& images){
    ifstream file(filename, ios::binary);
    if (file.is_open()) {
        int magic_number = 0;
        int number_of_images = 0;
        int n_rows = 0;
        int n_cols = 0;
        unsigned char label;
        file.read((char *) &magic_number, sizeof(magic_number));
        file.read((char *) &number_of_images, sizeof(number_of_images));
        file.read((char *) &n_rows, sizeof(n_rows));
        file.read((char *) &n_cols, sizeof(n_cols));
        magic_number = ReverseInt(magic_number);
        number_of_images = ReverseInt(number_of_images);
        n_rows = ReverseInt(n_rows);
        n_cols = ReverseInt(n_cols);


        for (int i = 0; i < number_of_images; i++) {
            vector<real> tp;
            for (int r = 0; r < n_rows; r++) {
                for (int c = 0; c < n_cols; c++) {
                    unsigned char i = 0;
                    file.read((char *) &i, sizeof(i));
                    real pixel=(real)i/255.0f;
                    tp.push_back(pixel);
                }
            }
            images.push_back(tp);
        }
    }
}

void read_OnePic(string filename,vector<vector<int>>& image){
    ifstream file;
    file.open(filename,ios::in);
    if(file.is_open()){
        int magic = 0;
        int num = 0;
        int rows = 0;
        int cols = 0;
        unsigned char label;
        file.read((char *) &magic, sizeof(int));
        file.read((char *) &num, sizeof(int));
        file.read((char *) &rows, sizeof(int));
        file.read((char *) &cols, sizeof(int));
        magic = ReverseInt(magic);
        num = ReverseInt(num);
        rows = ReverseInt(rows);
        cols = ReverseInt(cols);
        image=vector<vector<int>> (rows,vector<int>(cols));
        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                unsigned char c;
                file.read((char *) &c, sizeof(char));
                image[i][j]=c;
            }
        }
    }
}
void read_onepic_poisson(string filename,vector<real>& image){
    ifstream file;
    file.open(filename,ios::in);
    if(file.is_open()){
        int magic = 0;
        int num = 0;
        int rows = 0;
        int cols = 0;
        unsigned char label;
        file.read((char *) &magic, sizeof(int));
        file.read((char *) &num, sizeof(int));
        file.read((char *) &rows, sizeof(int));
        file.read((char *) &cols, sizeof(int));
        magic = ReverseInt(magic);
        num = ReverseInt(num);
        rows = ReverseInt(rows);
        cols = ReverseInt(cols);
        image=vector<real>(rows*cols);
        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                unsigned char c;
                file.read((char *) &c, sizeof(char));
                real pixel=(real)c/255.0f;
                image[i*cols+j]=pixel;
            }
        }
    }
}
/**
 * @brief 读取IRIS数据集
 * 
 */
