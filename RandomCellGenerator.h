#pragma once

#include "Cell.h"

#include <random>

class RandomCellGenerator
{
public:
    explicit RandomCellGenerator(int size);

    Cell operator()();

private:
    std::mt19937 generator;
    std::uniform_int_distribution<int> distribution;
};
