#ifndef MATRIX_IO_HPP
#define MATRIX_IO_HPP
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <exception>

#include "matrix.hpp"

namespace mops {

template <typename T>
Matrix<T> read_dense_matrix(const std::string &filename) {
	std::ifstream file(filename);

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
	Matrix<T> matrix(rows,cols);
	
	for (auto &e : matrix) {
		file >> e;
	}
	file.close();
	return matrix;
}

template <typename T>
void write_dense_matrix(const Matrix<T> &matrix,
			const std::string &file_name) {
	std::ofstream file(file_name);
	file << "%%MatrixMarket matrix array real general" << std::endl;
	file << matrix.get_rows() << " " << matrix.get_cols() << std::endl;

	for (auto &e : matrix) {
		file << e << std::endl;
	}
	file.close();
}

}  // namespace mops
#endif
