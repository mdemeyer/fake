#include <chrono>
#include <future>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <string>

#include <boost/program_options.hpp>

class ProgressCounter
{
public:
    constexpr explicit ProgressCounter(unsigned int endValue = 100) noexcept
        : m_endValue(endValue)
        , m_value(0)
        , m_finished(false)
    {}

    unsigned int get() const
    {
        std::shared_lock<std::shared_mutex> lock(m_mutex);
        return m_value;
    }

    bool finished() const
    {
        std::shared_lock<std::shared_mutex> lock(m_mutex);
        return m_finished;
    }

    void increment()
    {
        std::unique_lock<std::shared_mutex> lock(m_mutex);
        unsigned int newValue = ++m_value;

        if(newValue == m_endValue) {
            m_finished = true;
        }
    }

private:
    mutable std::shared_mutex m_mutex;

    const unsigned int m_endValue;

    unsigned int m_value;
    bool m_finished;
};

int main(int ac, char** av)
{
    using namespace std::chrono_literals;
    namespace po = boost::program_options;

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

    ProgressCounter counter;
    std::shared_mutex coutMutex;
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

            std::unique_lock<std::shared_mutex> lock(coutMutex);
            std::cout << "[" << std::setw(3) << std::right << t  << "%] "
                      << "Building CXX object CMakeFiles/" << "fake name" << ".dir/src/main.cpp.o"
                      << std::endl;
            lock.unlock();

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
