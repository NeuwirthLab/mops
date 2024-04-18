#include "include/mops.hpp"

using ValueType = double;

int main(int argc, char** argv){
	mops::Matrix<double>A = mops::read_dense_matrix<ValueType>("mat.mtx");
	for(auto &a : A)
		std::cout << a << std::endl;
	return 0;
}
