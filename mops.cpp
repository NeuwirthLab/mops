#include <getopt.h>
#include <omp.h>
#include <vector>
#include <chrono>
#include <random>
#include <filesystem>
#include <algorithm>

#include "mops.hpp"

namespace fs = std::filesystem;
using ValueType = double;
using Clock = std::chrono::high_resolution_clock;
using Duration = std::chrono::duration<double>;
static int copy_flag = false;
static int tacoed = false;


int main(int argc, char** argv){
	fs::path input_file{};
	fs::path output_file{};
    int cols, rows;
	int threads = std::thread::hardware_concurrency();

	const char* const short_opts = "i:o:t:c:r";

	static const struct option long_opts[] = {
	    {"input", required_argument, nullptr, 'i'},
	    {"output", required_argument, nullptr, 'o'},
	    {"threads", required_argument, nullptr, 't'},
        {"cols",    required_argument, nullptr, 'c'},
        {"rows",    required_argument, nullptr, 'r'},
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
		switch (opt) {
			case 'i':
				input_file = optarg;
				break;
			case 'o':
				output_file = optarg;
				break;
			case 't':
				omp_set_num_threads(std::atoi(optarg));
				break;
			default:
				std::cerr << "Parameter unsupported\n";
				std::terminate();
		}
	}
	
	auto t0 = Clock::now();
	mops::Matrix<ValueType>A = mops::read_dense_matrix<ValueType>(input_file);
	Duration d = Clock::now() - t0;

	std::cout << "Read time: " << d.count() << " [s]\n";
	std::cout << "Input file size: " << fs::file_size(input_file) << " [b]\n";
	std::cout << "Read BW: " << fs:.file_size(input_file) / d.count() << " [b/s]\n";

	std::vector<ValueType> x(A.get_cols());
	std::vector<ValueType> z(A.get_rows());
	std::vector<ValueType> y(A.get_rows());
	
	const ValueType alpha{42.0};
	const ValueType beta{33.0};
	
	std::default_random_engine(0);
	std::uniform_real_distribution<ValueType> unif(0,100);	
	auto generator_lambda = [&](){return unif(gen);};
	
	std::generate(x.begin(),x.end(),generator_lamda);
	std::generate(z.begin(),z.end(),generator_lamda);

			
	return 0;
}
