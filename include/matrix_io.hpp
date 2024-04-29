#ifndef MATRIX_IO_HPP
#define MATRIX_IO_HPP
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "matrix.hpp"

namespace mops {


template <typename T>
Matrix<T> read_dense_matrix(const std::string& file_name) {
	std::ifstream file(file_name);
        if (!file.is_open()) {
            std::cerr << "Error opening file." << std::endl;
            std::terminate();
        }

        std::string line;
        int rows, cols;

        std::getline(file, line);
        if (line.find("%%MatrixMarket matrix array") == std::string::npos) {
            std::cerr << "Invalid Matrix Market file." << std::endl;
            std::terminate();
        }

        do {
            std::getline(file, line);
        } while (line[0] == '%');

        std::istringstream iss(line);
        iss >> rows >> cols;
        Matrix<T> matrix(rows, cols);

        for (auto &e: matrix) {
            file >> e;
        }
        file.close();
        return matrix;
    }

    template<typename T>
    void write_dense_matrix(Matrix<T> &matrix, const std::string &file_name) {
        std::ofstream file(file_name);
        file << "%%MatrixMarket matrix array real general" << std::endl;
        file << matrix.get_rows() << " " << matrix.get_cols() << std::endl;

        for (auto &e: matrix) {
            file << e << std::endl;
        }
        file.close();
    }

    template<typename T>
    void write_dense_vector(std::vector<T> &vec,
                            const std::string &file_name) {
        std::ofstream file(file_name);
        file << "%%MatrixMarket matrix array real general" << std::endl;
        file << vec.size() << std::endl;

        for (auto &e: vec) {
            file << e << std::endl;
        }
        file.close();
    };

    template<typename T>
    void write_sparse_matrix(SparseMatrixCoo<T> &matrix, const std::string &file_name) {
        std::ofstream file(file_name);
        file << "%%MatrixMarket matrix coordinate real general" << std::endl;
        file << matrix.get_rows() << " " << matrix.get_cols() << " " << matrix.get_num_non_zero() << std::endl;
        for (auto i=0; i < matrix.get_num_non_zero(); ++i) {
            file << matrix.get_row(i) + 1 << " " << matrix.get_col(i) + 1 << " " << matrix.get_data(i) << std::endl;
        }
        /*
        auto begin = matrix.csr_begin();
        auto row_begin = std::get<0>(begin);
        auto col_begin = std::get<1>(begin);
        auto data_begin = std::get<2>(begin);

        auto end = matrix.csr_end();
        auto row_end = std::get<0>(end);
        auto col_end = std::get<1>(end);
        auto data_end = std::get<2>(end);

        for(auto it = data_begin; it != data_end; ++it){
            file << *row_begin + 1 << " " << *col_begin + 1 << " " << *it << std::endl;
            ++row_begin;
            ++col_begin;
        }
                for (int i = 0; i < matrix.get_num_non_zero(); ++i) {
                    file << row_begin[i]  << " " << col_begin[i]  << " " << data_begin[i] << std::endl;
                }

        */
    };

    void copy_file(const std::string &src, const std::string &dst) {
        std::ifstream src_file(src, std::ios::binary);
        std::ofstream dst_file(dst, std::ios::binary);
        dst_file << src_file.rdbuf();
    }
} // namespace mops
#endif
