// Standard: C++23
// Tested compiler: g++ 14.2.0

#include "Experiment.h"

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

int main()
{
    try
    {
        int n = 0;
        int m = 0;
        int repetitions = 0;

        std::cout << "Board size n: ";
        if (!(std::cin >> n))
        {
            throw std::runtime_error("n must be an integer.");
        }

        std::cout << "Number of selected cells m: ";
        if (!(std::cin >> m))
        {
            throw std::runtime_error("m must be an integer.");
        }

        std::cout << "Number of experiment repetitions: ";
        if (!(std::cin >> repetitions))
        {
            throw std::runtime_error(
                "The number of repetitions must be an integer."
            );
        }

        const Statistics statistics =
            runExperiments(n, m, repetitions);

        const long long totalCells = 1LL * n * n;

        const double ratio =
            static_cast<double>(m) /
            static_cast<double>(totalCells);

        std::cout << std::fixed << std::setprecision(3);

        std::cout
            << "\nFor n = " << n
            << ", m = " << m
            << ", m/n^2 = " << ratio
            << '\n';

        std::cout
            << "Mean free-zone size   = "
            << statistics.mean
            << '\n';

        std::cout
            << "Median free-zone size = "
            << statistics.median
            << "\n\n";

        const std::vector<StudyPoint> study =
            studyDependence(n, repetitions);

        std::cout << "Dependence on m/n^2:\n";

        std::cout
            << std::setw(10) << "m/n^2"
            << std::setw(10) << "m"
            << std::setw(18) << "mean"
            << std::setw(18) << "median"
            << '\n';

        for (const StudyPoint& point : study)
        {
            std::cout
                << std::setw(10) << point.ratio
                << std::setw(10) << point.m
                << std::setw(18) << point.meanFree
                << std::setw(18) << point.medianFree
                << '\n';
        }

        return 0;
    }
    catch (const std::exception& error)
    {
        std::cerr
            << "Error: "
            << error.what()
            << '\n';

        return 1;
    }
}