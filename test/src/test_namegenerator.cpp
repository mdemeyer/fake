#include <gtest/gtest.h>
#include "namegenerator.hpp"

using namespace fake;

TEST(NameGenerator, get_name)
{
    NameGenerator gen(123);
    EXPECT_EQ("Transformer", gen.get_name());
}
