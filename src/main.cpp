#include <chrono>
#include <future>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>

#include <boost/program_options.hpp>

#include "progresscounter.hpp"

using namespace std::chrono_literals;
namespace po = boost::program_options;

int main(int ac, char** av)
{

    po::options_description desc("Fake options");
    desc.add_options()
            ("help", "produce help message")
            ("jobs,j", po::value<int>()->default_value(4), "amount of parallel tasks");

    po::variables_map vm;

    try {
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    fake::ProgressCounter counter;
    std::mutex coutMutex;
    std::vector<std::future<void>> output_futures;
    const int numTasks = vm["jobs"].as<int>();

    auto count_progress_task = [&counter]() {
        while(!counter.finished())
        {
            counter.increment();
            std::this_thread::sleep_for(50ms);
        }
    };

    auto print_output_task = [&counter, &coutMutex](auto time) {
        while(!counter.finished())
        {
            auto t = counter.get();

            if(coutMutex.try_lock()) {
                std::cout << "[" << std::setw(3) << std::right << t  << "%] "
                          << "Building CXX object CMakeFiles/" << "fake name" << ".dir/src/main.cpp.o"
                          << std::endl;
                coutMutex.unlock();
            }

            std::this_thread::sleep_for(time);
        }
    };

    std::cout << "Scanning dependencies of target fake" << std::endl;

    auto progress_future = std::async(count_progress_task);

    for(int i = 0; i < numTasks; i++){
        output_futures.push_back(std::async(print_output_task, 100ms));
    }

    progress_future.wait();

    for(const auto& fut : output_futures)
    {
        fut.wait();
    }

    std::cout << "[100%] Linking CXX executable fake" << "\n"
              << "[100%] Built target fake"
              << std::endl;
    return 0;
}
