#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <vector>
#include <tuple>

namespace mops {

    enum class MatrixType {
        dense,
        sparse
    };

    template<typename T>
    class Matrix {
        std::vector<T> _data;
        int _rows;
        int _cols;

        auto inline element_access(const int i_, const int j_) {
            return _data[i_ * _cols + j_];
        }

    public:
        MatrixType type = MatrixType::dense;

        Matrix(const int rows_, const int cols_)
                : _rows{rows_}, _cols{cols_}, _data(rows_ * cols_) {}

        [[nodiscard]] inline const T &operator()(const int i_,
                                                 const int j_) const {
            return element_access(i_, j_);
        }

        [[nodiscard]] inline T &operator()(const int i_, const int j_) {
            return element_access(i_, j_);
        }

        [[nodiscard]] inline int get_rows() const { return _rows; }

        [[nodiscard]] inline int get_cols() const { return _cols; }

        inline auto begin() { return _data.begin(); }

        inline auto end() { return _data.end(); }

        inline auto data() { return _data.data(); }
    };


    template<typename T>
    class SparseMatrix {
        std::vector<int> _rows;
        std::vector<int> _cols;
        std::vector<T> _data;
        int _num_rows;
        int _num_cols;
        int _num_non_zero;

    public:
        MatrixType type = MatrixType::sparse;

        SparseMatrix(const int rows_, const int cols_, const int num_non_zero_)
                : _num_rows{rows_}, _num_cols{cols_}, _num_non_zero{num_non_zero_} {
            _rows.reserve(num_non_zero_ + 1);
            _cols.reserve(num_non_zero_);
            _data.reserve(num_non_zero_);
        }

        [[nodiscard]] inline const T &operator()(const int i_, const int j_) const {
            for (int i = 0; i < _num_non_zero; ++i) {
                if (_rows[i] == i_ && _cols[i] == j_) {
                    return _data[i];
                }
            }
            return 0;
        }

        [[nodiscard]] inline T &operator()(const int i_, const int j_) {
            for (int i = 0; i < _num_non_zero; ++i) {
                if (_rows[i] == i_ && _cols[i] == j_) {
                    return _data[i];
                }
            }
            return 0;
        }

        [[nodiscard]] inline int get_num_non_zero() const { return _num_non_zero; }

        inline auto begin() { return _data.begin(); }

        inline auto end() { return _data.end(); }

        inline auto data() { return _data.data(); }

        inline auto rows() { return _rows.data(); }

        inline auto cols() { return _cols.data(); }

        inline auto csr_begin() { return {_rows.begin(), _cols.begin(), _data.begin()}; }

        inline void push_back(int row, int col, double value) {
            _rows.push_back(row);
            _cols.push_back(col);
            _data.push_back(value);
        }
    };
} // namespace mops
#endif
