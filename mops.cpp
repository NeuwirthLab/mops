#include <getopt.h>
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <random>
#include <thread>
#include <vector>

#include "mops.hpp"

#ifdef WITH_TACO
#include "taco.h"
#endif

#if defined(WITH_LIKWID) || defined(WITH_OPENMP)
#include <omp.h>
#endif
#ifdef WITH_LIKWID
#include <likwid-marker.h>
#endif

namespace fs = std::filesystem;
using ValueType = double;
using Clock = std::chrono::high_resolution_clock;
using Duration = std::chrono::duration<double>;
static int copy_flag = false;
static int tacoed = false;
static int generator = false;
static int is_sparse = false;


int main(int argc, char** argv) {
    fs::path input_file{};
    fs::path output_file{};
    fs::path write_file_path{};
    fs::path read_file_path{};

    int cols, rows;
    int threads = std::thread::hardware_concurrency();

    const char *const short_opts = "i:o:c:r:l:s:t:";

    static const struct option long_opts[] = {
            {"input",     required_argument, nullptr,    'i'},
            {"output",    required_argument, nullptr,    'o'},
            {"threads",   required_argument, nullptr,    't'},
            {"cols",      required_argument, nullptr,    'c'},
            {"rows",      required_argument, nullptr,    'r'},
            {"generator", no_argument,       &generator, true},
            {"g_read",    required_argument, nullptr,    'l'},
            {"g_write",   required_argument, nullptr,    's'},
            {"copy",      no_argument,       &copy_flag, true},
            {"tacoed",    no_argument,       &tacoed,    true},
            {"is_sparse", no_argument,       &is_sparse, true},

    };

    while (true) {
        int option_index = 0;
        const auto opt =
                getopt_long(argc, argv, short_opts, long_opts, nullptr);
        if (opt == -1)
            break;
        switch (opt) {
            case 0:
                if (long_opts[option_index].flag != nullptr)
                    break;
                if (optarg)
                    printf(" with arg %s", optarg);
                printf("\n");
                break;
            case 'i':
                input_file = optarg;
                break;
            case 'o':
                output_file = optarg;
                break;
            case 'l':
                read_file_path = optarg;
                break;
            case 's':
                write_file_path = optarg;
                break;
            case 'c':
                cols = std::stoi(optarg);
                break;
            case 'r':
                rows = std::stoi(optarg);
                break;
            case 't':

#if defined(WITH_LIKWID) || defined(WITH_OPENMP)
                omp_set_num_threads(std::atoi(optarg));
#endif
                break;
            default:
                std::cerr << "Parameter unsupported\n";
                std::cout << optarg << std::endl;
                std::terminate();
        }
    }
    if (is_sparse) {
        int nnz = mops::getNumNonZero(rows, cols, 0.1);
        if (generator) {
            std::string filename =
                    mops::get_file_name_from_rows_and_cols(rows, cols, false);
            write_file_path += filename;
            read_file_path += filename;

            if (!copy_flag) {
                std::cout << "Writing the matrix file to: " << write_file_path
                          << std::endl;

                mops::SparseMatrixCoo<double> m = mops::generate_sparse_matrix_coo<double>(rows, cols, nnz, 1.0);
#ifdef DEBUG
                auto t_begin = std::chrono::high_resolution_clock::now();
#endif
                mops::write_sparse_matrix(m, write_file_path);

#ifdef DEBUG
                auto t_end = std::chrono::high_resolution_clock::now();
                auto duration =
                        std::chrono::duration_cast<std::chrono::duration<double>>(
                                t_end - t_begin);
                mops::printFileStats(write_file_path, rows, cols, duration);
#endif

            } else {
                std::cout << "Copying the matrix file from: " << read_file_path
                          << " to " << write_file_path << std::endl;
                mops::SparseMatrixCoo<double> m = mops::generate_sparse_matrix_coo<double>(rows, cols, nnz, 1.0);
                mops::write_sparse_matrix(m, write_file_path);
#ifdef DEBUG
                auto t_begin = std::chrono::high_resolution_clock::now();
#endif
                copy_file(read_file_path, write_file_path);
#ifdef DEBUG
                auto t_end = std::chrono::high_resolution_clock::now();
                auto duration =
                        std::chrono::duration_cast<std::chrono::duration<double>>(
                                t_end - t_begin);
                mops::printFileStats(write_file_path, rows, cols, duration);
#endif
            }
        } else {
            // do the matrix vector multiplication
        }
        } else {
            if (generator) {
                std::string filename =
                        mops::get_file_name_from_rows_and_cols(rows, cols, true);
                write_file_path += filename;
                read_file_path += filename;

                if (!copy_flag) {
                    std::cout << "Writing the matrix file to: " << write_file_path
                              << std::endl;
                    if (tacoed) {
        #ifdef WITH_TACO
                        taco::Tensor<double> m =
                            mops::taco_generate_dense_matrix<double>(rows, cols, 1);

        #if defined(BENCHMARK)
                        auto t_begin = std::chrono::high_resolution_clock::now();
        #endif
                        taco::write(write_file_path, m);
        #if defined(BENCHMARK)
                        auto t_end = std::chrono::high_resolution_clock::now();
                        auto duration =
                            std::chrono::duration_cast<std::chrono::duration<double>>(
                                t_end - t_begin);
                        mops::printFileStats(write_file_path, rows, cols, duration);
        #endif
        #endif
                    } else {
                        mops::Matrix<double> m =
                                mops::generate_dense_matrix<double>(rows, cols, 1);
        #ifdef DEBUG
                        auto t_begin = std::chrono::high_resolution_clock::now();
        #endif
                        mops::write_dense_matrix(m, write_file_path);
        #ifdef DEBUG
                        auto t_end = std::chrono::high_resolution_clock::now();
                        auto duration =
                                std::chrono::duration_cast<std::chrono::duration<double>>(
                                        t_end - t_begin);
                        mops::printFileStats(write_file_path, rows, cols, duration);
        #endif
                    }
                } else {
                    std::cout << "Copying the matrix file from: " << read_file_path
                              << " to " << write_file_path << std::endl;
                    if (tacoed) {
        #ifdef WITH_TACO
                        taco::Tensor<double> m =
                            mops::taco_generate_dense_matrix<double>(rows, cols, 1);
                        taco::write(read_file_path, m);
        #endif
                    } else {
                        mops::Matrix<double> matrix =
                                mops::generate_dense_matrix<double>(rows, cols, 1);
                        mops::write_dense_matrix(matrix, read_file_path);
                    }
        #ifdef DEBUG
                    auto t_begin = std::chrono::high_resolution_clock::now();
        #endif
                    copy_file(read_file_path, write_file_path);
        #ifdef DEBUG
                    auto t_end = std::chrono::high_resolution_clock::now();
                    auto duration =
                            std::chrono::duration_cast<std::chrono::duration<double>>(
                                    t_end - t_begin);
                    mops::printFileStats(write_file_path, rows, cols, duration);
        #endif
                }
            } else {
#ifdef DEBUG
                auto t0 = Clock::now();
#endif
                mops::Matrix<ValueType> A =
                        mops::read_dense_matrix<ValueType>(input_file);
#ifdef DEBUG
                Duration d = Clock::now() - t0;

                std::cout << "Read time: " << d.count() << " [s]\n";
                std::cout << "Input file size: " << fs::file_size(input_file)
                          << " [b]\n";
                std::cout << "Read BW: "
                          << static_cast<double>(fs::file_size(input_file)) / d.count()
                          << " [b/s]\n";
#endif
                std::vector<ValueType> x(A.get_cols());
                std::vector<ValueType> y(A.get_rows());
                std::vector<ValueType> z(A.get_rows());

                const ValueType alpha{42.0};
                const ValueType beta{33.0};

                std::default_random_engine rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<ValueType> unif(0, 100);
                auto generator_lambda = [&]() { return unif(gen); };

                std::generate(x.begin(), x.end(), generator_lambda);
                std::generate(z.begin(), z.end(), generator_lambda);

#ifdef WITH_LIKWID
                LIKWID_MARKER_INIT;

#pragma omp parallel
		{
			LIKWID_MARKER_THREADINIT;
			LIKWID_MARKER_REGISTER("mat_vec");
		}

#pragma omp parallel
		{
			LIKWID_MARKER_START("mat_vec");
			mops::mat_vec(alpha, beta, y, A, x, z);
			LIKWID_MARKER_STOP("mat_vec");
		}
		LIKWID_MARKER_CLOSE;
#else
                mops::mat_vec(alpha, beta, y, A, x, z);
#endif
#ifdef DEBUG
                t0 = Clock::now();
#endif
                mops::write_dense_vector(y, output_file);
#ifdef DEBUG
                d = Clock::now() - t0;

                std::cout << "Write time: " << d.count() << " [s]\n";
                std::cout << "Output file size: " << fs::file_size(output_file)
                          << " [b]\n";
                std::cout << "Write BW: "
                          << static_cast<double>(fs::file_size(input_file)) / d.count()
                          << " [b/s]\n";
#endif
            }

        }
    return 0;
}
