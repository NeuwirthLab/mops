#ifndef MATRIX_GENERATOR_HPP
#define MATRIX_GENERATOR_HPP
#include <iostream>
#include <random>
#include <type_traits>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include "matrix.hpp"
#ifdef WITH_TACO
#include "taco.h"
#endif


namespace mops {
    template<typename T>
    Matrix<T> generate_dense_matrix(int rows, int cols, int max_value) {
        Matrix<T> matrix(rows, cols);
        std::random_device rd;
        std::mt19937 gen(rd());
        if (std::is_same<T, int>()) {
            std::uniform_int_distribution<int> val_dist(0, max_value);
            std::generate(matrix.begin(), matrix.end(), [&val_dist, &gen]() { return val_dist(gen); });
        } else {
            std::uniform_real_distribution<T> val_dist(0, max_value);
            std::generate(matrix.begin(), matrix.end(), [&val_dist, &gen]() { return val_dist(gen); });
        }
        return matrix;
    }

    template<typename T>
    SparseMatrix<T> generate_sparse_matrix(int rows, int cols, T max_value, int non_zeros) {
        SparseMatrix<T> matrix(rows, cols, non_zeros);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> row_dist(0, rows - 1);
        std::uniform_int_distribution<int> col_dist(0, cols - 1);
        if constexpr (std::is_integral<T>::value) {
            std::uniform_int_distribution<T> val_dist(0, max_value);
            for (int i = 0; i < non_zeros; ++i) {
                matrix.push_back(row_dist(gen), col_dist(gen), val_dist(gen));
            }
        } else {
            std::uniform_real_distribution<T> val_dist(0, max_value);
            for (int i = 0; i < non_zeros; ++i) {
                matrix.push_back(row_dist(gen), col_dist(gen), val_dist(gen));
            }
        }
        return matrix;
    }

    template<typename T>
    SparseMatrix<T> generate_random_number_vector(int nnz, int cols, int rows, T max_value){
        std::vector<int> col_indices(cols);
        std::vector<int> row_indices(rows);
        SparseMatrix<T> matrix(rows, cols, nnz);
        std::random_device rd;
        std::mt19937 gen(rd());

        std::iota(col_indices.begin(), col_indices.end(), 0);
        std::iota(row_indices.begin(), row_indices.end(), 0);
        std::shuffle(col_indices.begin(), col_indices.end(), std::mt19937(std::random_device()()));
        std::shuffle(row_indices.begin(), row_indices.end(), std::mt19937(std::random_device()()));

        if (std::is_same<T, int>()) {
            std::uniform_int_distribution<T> val_dist(0, max_value);
            for(int i= 0; i < nnz; ++i){
                matrix.push_back(row_indices[i], col_indices[i], val_dist(gen));
            }
        }else {
            std::uniform_real_distribution<T> val_dist(0, max_value);
            for(int i= 0; i < nnz; ++i){
                matrix.push_back(row_indices[i], col_indices[i], val_dist(gen));
        }
        return matrix;
    }

#ifdef WITH_TACO
        template<typename T>
        taco::Tensor<T> taco_generate_dense_matrix(int rows, int cols, int max_value) {
            taco::Format dm({taco::Dense, taco::Dense});
            taco::Tensor<T> m({rows, cols}, dm);
            std::random_device rd;
            std::mt19937 gen(rd());
            if (std::is_same<T, int>::value) {
                std::uniform_int_distribution<int> val_dist(0, max_value);
                for (int i = 0; i < rows; ++i) {
                    for (int j = 0; j < cols; ++j) {
                        m.insert({i, j}, val_dist(gen));
                    }
                }
            } else {
                std::uniform_real_distribution<T> val_dist(0, max_value);
                for (int i = 0; i < rows; ++i) {
                    for (int j = 0; j < cols; ++j) {
                        m.insert({i, j}, val_dist(gen));
                    }
                }
            }

            m.pack();
            return m;
        }
#endif
    }
}
#endif
