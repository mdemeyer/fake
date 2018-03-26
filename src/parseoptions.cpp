#include "parseoptions.hpp"

#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace fake
{

Options read_options(int ac, char** av)
{
    bool valid = true;
    int numTasks = 4;

    po::options_description desc("Fake options");
    desc.add_options()
            ("help", "produce help message")
            ("jobs,j", po::value<int>()->default_value(numTasks), "amount of parallel tasks");

    po::variables_map vm;

    try {
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        valid = false;
    }

    if (vm.count("help")) {
        std::cout << desc << "\n";
        valid = false;
    }

    if(valid) {
        numTasks = vm["jobs"].as<int>();
    }

    return { valid, numTasks };
}


}
