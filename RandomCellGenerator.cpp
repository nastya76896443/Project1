#include "RandomCellGenerator.h"

#include <stdexcept>

RandomCellGenerator::RandomCellGenerator(int size) :
      generator(std::random_device{}()),
      distribution(0, size > 0 ? size - 1 : 0)
{
    if (size <= 0)
    {
        throw std::invalid_argument("Board size n must be positive.");
    }
}

Cell RandomCellGenerator::operator()()
{
    const int row = distribution(generator);
    const int col = distribution(generator);

    return {row, col};
}
