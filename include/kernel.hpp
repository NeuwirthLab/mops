#ifndef KERNEL_HPP
#define KERNEL_HPP
#include <vector>
#include "matrix.hpp"

namespace mops {
template <typename T>
void mat_vec(T alpha,
             T beta,
             std::vector<T>* y,
             Matrix<T>* A,
             std::vector<T>* x,
             std::vector<T>* z) {
	int y1_dimension = y->size();
	int A1_dimension = A->get_rows();
	int A2_dimension = A->get_cols();
	int x1_dimension = x->size();
	int z1_dimension = z->size();
	T* A_vals = A->data();
	T* y_vals = y->data();
	T* x_vals = x->data();
	T* z_vals = z->data();
#if defined(WITH_LIKWID) || defined(WITH_OPENMP)
#pragma omp parallel for schedule(runtime)
#endif
	for (int i = 0; i < z1_dimension; ++i) {
		T tj_val = 0.0;
		for (int j = 0; j < x1_dimension; ++j) {
			int jA = i * A2_dimension + j;
			tj_val += alpha * (A_vals[jA] * x_vals[j]);
		}
		y_vals[i] = tj_val + beta * z_vals[i];
	}
}
} // namespace mops
#endif
