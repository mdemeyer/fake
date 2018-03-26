#pragma once

namespace fake
{

struct Options
{
    Options(bool _valid, int _num_tasks)
        : valid(_valid)
        , num_tasks(_num_tasks)
    {}

    const bool valid;
    const int num_tasks;
};

Options read_options(int ac, char** av);

}
