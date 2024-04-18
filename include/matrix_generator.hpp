#ifndef MATRIX_GENERATOR_HPP
#define MATRIX_GENERATOR_HPP
#include <iostream>
#include <vector>
#include <random>
#include <type_traits>
#include <fstream>
#include <iomanip>
#include <getopt.h>
#include <filesystem>

#ifdef WITH_TACO
#include "taco.h"
#endif
namespace mops{}
//template<typename T>
//std::vector<T> generate_dense_matrix(int rows, int cols, int max_value) {
//    std::vector<T> matrix(rows * cols, 0);
//    std::random_device rd;
//    std::mt19937 gen(rd());
//    if(std::is_same<T, int>::value) {
//        std::uniform_int_distribution<int> val_dist(0, max_value);
//        std::generate(matrix.begin(), matrix.end(), [&val_dist, &gen]() { return val_dist(gen); });
//    }else{
//        std::uniform_real_distribution<T> val_dist(0, max_value);
//        std::generate(matrix.begin(), matrix.end(), [&val_dist, &gen]() { return val_dist(gen); });
//    }
//    return matrix;
//}
//
//
//template<typename T>
//taco::Tensor<double> taco_generate_dense_matrix(int rows, int cols, int num_non_zero, int max_value) {
//    taco::Format dm({taco::Dense,taco::Dense});
//    taco::Tensor<double> m({rows,cols},   dm);
//    std::random_device rd;
//    std::mt19937 gen(rd());
//    if (std::is_same<T, int>::value) {
//        std::uniform_int_distribution<int> val_dist(0, max_value);
//        for (int i = 0; i < rows; ++i) {
//            for (int j = 0; j < cols; ++j) {
//                m.insert({i,j},val_dist(gen));
//            }
//        }
//    } else {
//        std::uniform_real_distribution<T> val_dist(0, max_value);
//        for (int i = 0; i < rows; ++i) {
//            for (int j = 0; j < cols; ++j) {
//                m.insert({i,j},val_dist(gen));
//            }
//        }
//    }
//
//    m.pack();
//    return m;
//}

#endif
