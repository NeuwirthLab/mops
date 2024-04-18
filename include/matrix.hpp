#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <vector>

namespace mops {
template <typename T>
class Matrix {
	std::vector<T> _data;
	int _rows;
	int _cols;
	auto inline element_access(const int i_, const int j_) {
		return _data[i_ * _cols + j_];
	}

       public:
	Matrix(const int rows_, const int cols_)
	    : _rows{rows_}, _cols{cols_}, _data(rows_ * cols_) {}
	[[nodiscard]] inline const T& operator()(const int i_,
						 const int j_) const {
		return element_access(i_, j_);
	}
	[[nodiscard]] inline T& operator()(const int i_, const int j_) {
		return element_access(i_, j_);
	}

	[[nodiscard]] inline int get_rows() const { return _rows; }
	[[nodiscard]] inline int get_cols() const { return _cols; }
	inline auto begin() const { return _data.begin(); }
	inline auto end() { return _data.end(); }
	inline auto data() {return _data.data();}
};
}  // namespace mops
#endif
