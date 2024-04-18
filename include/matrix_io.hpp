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

	for (auto& e : matrix) {
		file >> e;
	}
	file.close();
	return matrix;
}

template <typename T>
void write_dense_matrix(const Matrix<T>& matrix, const std::string& file_name) {
	std::ofstream file(file_name);
	file << "%%MatrixMarket matrix array real general" << std::endl;
	file << matrix.get_rows() << " " << matrix.get_cols() << std::endl;

	for (auto& e : matrix) {
		file << e << std::endl;
	}
	file.close();
}

template <typename T>
void write_dense_vector(const std::vector<T>& vec,
                        const std::string& file_name) {
	std::ofstream file(file_name);
	file << "%%MatrixMarket matrix array real general" << std::endl;
	file << vec.size() << std::endl;

	for (auto& e : vec) {
		file << e << std::endl;
	}
	file.close();
}

void copy_file(const std::string& src, const std::string& dst) {
	std::ifstream src_file(src, std::ios::binary);
	std::ofstream dst_file(dst, std::ios::binary);
	dst_file << src_file.rdbuf();
}

} // namespace mops
#endif
