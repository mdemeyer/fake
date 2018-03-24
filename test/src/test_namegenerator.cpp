#include <gtest/gtest.h>
#include "namegenerator.hpp"

using namespace fake;

TEST(NameGenerator, get_name)
{
    NameGenerator gen(123);
    EXPECT_EQ("Transformer", gen.get_name());
}

TEST(NameGenerator, singleton_bingo)
{
    NameGenerator gen(29905798);
    EXPECT_EQ("SingletonSingletonSingleton", gen.get_name());
}
