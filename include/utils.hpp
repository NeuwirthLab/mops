#ifndef UTILS_HPP
#define UTILS_HPP
#include <iostream>
#include <fstream>
#include <filesystem>


std::string get_file_name_from_rows_and_cols(int rows, int cols, bool is_dense) {
    if (is_dense) {
        return "dense_matrix_" + std::to_string(rows) + "x" + std::to_string(cols) + ".mtx";
    } else {
        return "sparse_matrix_" + std::to_string(rows) + "x" + std::to_string(cols) + ".mtx";
    }
}

std::ifstream::pos_type get_filesize(const char *filename) {
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

void printFileStats(const std::string& write_file_path, int rows, int cols, const std::chrono::duration<double>& duration) {
    auto f_size = get_filesize(write_file_path.c_str());
    std::cout << "Elapsed time: " << duration.count() << " seconds" << std::endl;
    std::cout << "Matrix dimensions: " << rows << "x" << cols << std::endl;
    std::cout << "File size (Byte): " << f_size << std::endl;
    std::cout << "Bandwidth (Byte/s): " << f_size / duration.count() << std::endl;
}




#endif
