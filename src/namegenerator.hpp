#pragma once

#include <random>
#include <string>
#include <vector>

namespace fake {

class NameGenerator
{
public:
    NameGenerator();
    explicit NameGenerator(unsigned int seed);

    std::string get_name();

private:
    std::mt19937 m_twister;

    using vector_size_t = std::vector<std::string>::size_type;
    std::uniform_int_distribution<vector_size_t> m_num_words_dist;
    std::uniform_int_distribution<vector_size_t> m_words_dist;
};

}
