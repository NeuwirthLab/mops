#include <getopt.h>
//#include <omp.h>
#include <vector>
#include <chrono>
#include <random>
#include <filesystem>
#include <algorithm>
#include <iostream>

#include "mops.hpp"

namespace fs = std::filesystem;
using ValueType = double;
using Clock = std::chrono::high_resolution_clock;
using Duration = std::chrono::duration<double>;
static int copy_flag = false;
static int tacoed = false;
static int generator = false;


int main(int argc, char** argv){
	fs::path input_file{};
	fs::path output_file{};
    fs::path write_file_path{};
    fs::path read_file_path{};

    int cols, rows;
	//int threads = std::thread::hardware_concurrency();

	const char* const short_opts = "i:o:t:c:r";

	static const struct option long_opts[] = {
	    {"input", required_argument, nullptr, 'i'},
	    {"output", required_argument, nullptr, 'o'},
	    {"threads", required_argument, nullptr, 't'},
        {"cols",    required_argument, nullptr, 'c'},
        {"rows",    required_argument, nullptr, 'r'},
        {"generator", no_argument, &generator, true},
        {"g_read", required_argument, nullptr, 'l'},
        {"g_write", required_argument, nullptr, 's'},
        {"copy", no_argument, &copy_flag, true},
        {"tacoed", no_argument, &tacoed, true},
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
                    printf (" with arg %s", optarg);
                printf ("\n");
                break;
			case 'i':
				input_file = optarg;
				break;
			case 'o':
				output_file = optarg;
				break;
			case 't':
				//omp_set_num_threads(std::atoi(optarg));
				break;
            case 'l':
                read_file_path = optarg;
                break;
            case 's':
                write_file_path = optarg;
                break;
			default:
				std::cerr << "Parameter unsupported\n";
				std::terminate();
		}
	}

    bool generator=false;
    if (generator) {
        std::string filename = mops::get_file_name_from_rows_and_cols(rows, cols, true);
        write_file_path += filename;
        read_file_path += filename;

        if (!copy_flag) {
            std::cout << "Writing the matrix file to: " << write_file_path << std::endl;
            if (tacoed){
                 taco::Tensor<double> m = mops::taco_generate_dense_matrix<double>(rows, cols,1);
#if defined(BENCHMARK)
                auto t_begin = std::chrono::high_resolution_clock::now();
#endif
                taco::write(write_file_path, m);
#if defined(BENCHMARK)
                auto t_end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(t_end - t_begin);
            printFileStats(write_file_path, rows, cols, duration);
#endif
            }else{
                mops::Matrix<double> m = null;
#if defined(BENCHMARK)
                auto t_begin = std::chrono::high_resolution_clock::now();
#endif
                mops::write_dense_matrix(m, write_file_path);
#if defined(BENCHMARK)
                auto t_end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(t_end - t_begin);
            printFileStats(write_file_path, rows, cols, duration);
#endif
            }

        } else {
            std::cout << "Copying the matrix file from: " << read_file_path << " to "<< write_file_path << std::endl;
            if (tacoed){
                taco::Tensor<double> m = mops::taco_generate_dense_matrix<double>(rows, cols, 1);
                taco::write(read_file_path, m);
            }else {
                mops::Matrix<double> matrix = mops::generate_dense_matrix<double>(rows, cols,1);
                mops::write_dense_matrix(matrix, read_file_path);

            }
#if defined(BENCHMARK)
            auto t_begin = std::chrono::high_resolution_clock::now();
#endif
            copy_file(read_file_path, write_file_path);
#if defined(BENCHMARK)
            auto t_end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(t_end - t_begin);
        printFileStats(write_file_path, rows, cols, duration);
#endif
        }
    }
	return 0;
}
