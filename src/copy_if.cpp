
#include <iostream>
#include <fstream>
#include <algorithm>
#include "hpx/hpx.hpp"
#include "hpx/hpx_main.hpp"

#include "utilities.hpp"



int main(int argc, char* argv[])
{
	//handle command line arguements

	if (argc < 2) {
		std::cout << "Command line arguements not set" << std::endl;
		std::cout << "(vector size, iterations)" << std::endl;
		return 1;
	}

	int vector_size = std::stoi(argv[1]);
	int iterations = std::stoi(argv[2]);


	//initialize stuff

	auto threads = hpx::get_os_thread_count();

	utilities::timer timer;
	utilities::random_vector_generator gen;

	double std_seq_time = 0;
	double hpx_seq_time = 0;
	double hpx_par_time = 0;

	auto pred = [](double num) {return ((int)num % 4) < 2; };

	//run std sequential

	for (int i = 0; i < iterations; i++) {
		auto vec1 = gen.get_doubles(vector_size);
		decltype(vec1) vec2(vec1.size());

		timer.start();
		std::copy_if(vec1.begin(), vec1.end(), vec2.begin(), pred);
		timer.stop();
		//use result otherwise compiler will optimize it away:
		if (hpx::count(vec2.begin(), vec2.end(), 42)) std::cout << "wow";

		std_seq_time += timer.get();
	}
	std_seq_time /= iterations;


	//run hpx sequential

	for (int i = 0; i < iterations; i++) {
		auto vec1 = gen.get_doubles(vector_size);
		decltype(vec1) vec2(vec1.size());

		timer.start();
		hpx::copy_if(vec1.begin(), vec1.end(), vec2.begin(), pred);
		timer.stop();
		//use result otherwise compiler will optimize it away:
		if (hpx::count(vec2.begin(), vec2.end(), 42)) std::cout << "wow";

		hpx_seq_time += timer.get();
	}
	hpx_seq_time /= iterations;

	//run hpx parallel

	for (int i = 0; i < iterations; i++) {
		auto vec1 = gen.get_doubles(vector_size);
		decltype(vec1) vec2(vec1.size());

		timer.start();
		hpx::copy_if(hpx::execution::par, vec1.begin(), vec1.end(), vec2.begin(), pred);
		timer.stop();
		//use result otherwise compiler will optimize function call away:
		if (hpx::count(vec2.begin(), vec2.end(), 42)) std::cout << "wow";

		hpx_par_time += timer.get();
	}
	hpx_par_time /= iterations;



	//Write results to file:
	utilities::csv_result_file file("copy_if");
	file.append(vector_size, threads, std_seq_time, hpx_seq_time, hpx_par_time);


	return 0;
}
