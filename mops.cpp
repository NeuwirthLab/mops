#include <getopt.h>
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <random>
#include <thread>
#include <vector>

#include "mops.hpp"

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

int main(int argc, char** argv) {
	fs::path input_file{};
	fs::path output_file{};
	int threads = std::thread::hardware_concurrency();

	const char* const short_opts = "i:o:t:";

	static const struct option long_opts[] = {
	    {"input", required_argument, nullptr, 'i'},
	    {"output", required_argument, nullptr, 'o'},
	    {"threads", required_argument, nullptr, 't'},
	};

	while (true) {
		const auto opt =
		    getopt_long(argc, argv, short_opts, long_opts, nullptr);
		if (opt == -1)
			break;
		switch (opt) {
			case 'i':
				input_file = optarg;
				break;
			case 'o':
				output_file = optarg;
				break;
			case 't':
#if defined(WITH_LIKWID) || defined(WITH_OPENMP)
				omp_set_num_threads(std::atoi(optarg));
#endif
				break;
			default:
				std::cerr << "Parameter unsupported\n";
				std::terminate();
		}
	}

	auto t0 = Clock::now();
	mops::Matrix<ValueType> A = mops::read_dense_matrix<ValueType>(input_file);
	Duration d = Clock::now() - t0;

	std::cout << "Read time: " << d.count() << " [s]\n";
	std::cout << "Input file size: " << fs::file_size(input_file) << " [b]\n";
	std::cout << "Read BW: " << fs::file_size(input_file) / d.count()
	          << " [b/s]\n";

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
		mops::mat_vec(alpha, beta, &y, &A, &x, &z);
		LIKWID_MARKER_STOP("mat_vec");
	}
	LIKWID_MARKER_CLOSE;
#else
	mops::mat_vec(alpha, beta, &y, &A, &x, &z);
#endif

	t0 = Clock::now();
	mops::write_dense_vector(y, output_file);
	d = Clock::now() - t0;

	std::cout << "Write time: " << d.count() << " [s]\n";
	std::cout << "Output file size: " << fs::file_size(output_file) << " [b]\n";
	std::cout << "Write BW: " << fs::file_size(output_file) / d.count()
	          << " [b/s]\n";

	return 0;
}
