#include "Experiment.h"
#include "RandomCellGenerator.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace
{
int experimentImpl(int n, int m, RandomCellGenerator& randomCell)
{
    std::vector<std::vector<bool>> isNeighbor(
        n,
        std::vector<bool>(n, false)
    );

    const auto insideBoard = [n](int row, int col)
    {
        return row >= 0 && row < n && col >= 0 && col < n;
    };

    for (int i = 0; i < m; ++i)
    {
        const Cell selected = randomCell(); 

        for (int dRow = -1; dRow <= 1; ++dRow)
        {
            for (int dCol = -1; dCol <= 1; ++dCol)
            {

                const int row = selected.row + dRow;
                const int col = selected.col + dCol;

                if (insideBoard(row, col))
                {
                    isNeighbor[row][col] = true;
                }
            }
        }
    }

    return std::accumulate(
        isNeighbor.begin(),
        isNeighbor.end(),
        0,
        [](int total, const std::vector<bool>& row)
        {
            return total + static_cast<int>(std::count(row.begin(), row.end(), false));
        }
    );
}

void validateParameters(int n, int m, int repetitions)
{
    if (n <= 0)
    {
        throw std::invalid_argument("n must be positive.");
    }
    if (m < 0)
    {
        throw std::invalid_argument("m cannot be negative.");
    }
    if (repetitions <= 0)
    {
        throw std::invalid_argument("The number of repetitions must be positive.");
    }
}
} 

int experiment(int n, int m)
{
    validateParameters(n, m, 1);
    RandomCellGenerator randomCell(n);
    return experimentImpl(n, m, randomCell);
}

Statistics runExperiments(int n, int m, int repetitions)
{
    validateParameters(n, m, repetitions);

    RandomCellGenerator randomCell(n);
    std::vector<int> results;
    results.reserve(static_cast<std::size_t>(repetitions));

    for (int i = 0; i < repetitions; ++i)
    {
        results.push_back(experimentImpl(n, m, randomCell));
    }

    const double sum = std::accumulate(
        results.begin(),
        results.end(),
        0.0,
        std::plus<double>{}
    );
    const double mean = sum / static_cast<double>(repetitions);

    std::ranges::sort(results.begin(), results.end(), std::less<int>{});

    double median = 0.0;
    const std::size_t middle = results.size() / 2;

    if (results.size() % 2 == 1)
    {
        median = static_cast<double>(results[middle]);
    }
    else
    {
        median = (static_cast<double>(results[middle - 1]) +
                  static_cast<double>(results[middle])) / 2.0;
    }

    return {mean, median};
}

std::vector<StudyPoint> studyDependence(int n, int repetitions)
{
    validateParameters(n, 0, repetitions);

    const long long cellCount = 1LL * n * n;

    const std::vector<double> ratios{
        0.00, 0.05, 0.10, 0.20, 0.30, 0.40,
        0.50, 0.60, 0.80, 1.00, 1.25, 1.50
    };

    const auto ratioToM = [cellCount](double ratio)
    {
        const long long value = std::llround(ratio * static_cast<double>(cellCount));
        if (value > static_cast<long long>(std::numeric_limits<int>::max()))
        {
            throw std::overflow_error("m is too large for type int.");
        }
        return static_cast<int>(value);
    };

    std::vector<StudyPoint> result;
    result.reserve(ratios.size());

    for (double ratio : ratios)
    {
    const int currentM = ratioToM(ratio);

    const double actualRatio =
        static_cast<double>(currentM) /
        static_cast<double>(cellCount);

    const Statistics statistics =
    runExperiments(n, currentM, repetitions);

    result.push_back({
        actualRatio,
        currentM,
        statistics.mean,
        statistics.median
    });
        }

    return result;
}
