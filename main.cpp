// Standard: C++23
// Tested compiler: g++ 14.2.0

#include "Experiment.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

struct InputData
{
    int n = 0;
    int m = 0;
    int repetitions = 0;
};

bool isValid(const InputData& data)
{
    return data.n > 0
        && data.m >= 0
        && 1LL * data.m <= 1LL * data.n * data.n
        && data.repetitions > 0;
}

std::vector<InputData> readInputData(const char* fileName)
{
    std::ifstream fin(fileName);

    if (!fin.is_open())
    {
        return {};
    }

    std::vector<InputData> dataSets;
    InputData data;

    while (fin >> data.n >> data.m >> data.repetitions)
    {
        if (!isValid(data))
        {
            throw std::runtime_error("Incorrect data in input.txt");
        }

        dataSets.push_back(data);
    }

    if (!fin.eof())
    {
        throw std::runtime_error("Incorrect data in input.txt");
    }

    return dataSets;
}

void writeInputData(
    const char* fileName,
    const std::vector<InputData>& dataSets
)
{
    std::ofstream fout(fileName, std::ios::trunc);

    if (!fout.is_open())
    {
        throw std::runtime_error("Cannot write input.txt");
    }

    for (const InputData& data : dataSets)
    {
        fout
            << data.n << ' '
            << data.m << ' '
            << data.repetitions << '\n';
    }
}

std::vector<InputData> askForNewInputData()
{
    int count = 0;

    std::cout
        << "All input data have been used.\n"
        << "How many new data sets do you want to add? ";

    if (!(std::cin >> count) || count <= 0)
    {
        throw std::runtime_error("Incorrect number of data sets");
    }

    std::vector<InputData> dataSets;
    dataSets.reserve(count);

    for (int i = 0; i < count; ++i)
    {
        InputData data;

        std::cout
            << "Data set " << (i + 1)
            << " (n m repetitions): ";

        if (!(std::cin >> data.n >> data.m >> data.repetitions)
            || !isValid(data))
        {
            throw std::runtime_error("Incorrect input data");
        }

        dataSets.push_back(data);
    }

    return dataSets;
}

int main()
{
    try
    {
        std::vector<InputData> dataSets =
            readInputData("input.txt");

        if (dataSets.empty())
        {
            dataSets = askForNewInputData();
            writeInputData("input.txt", dataSets);
        }

        const InputData current = dataSets.front();

        const int n = current.n;
        const int m = current.m;
        const int repetitions = current.repetitions;

        const Statistics statistics =
            runExperiments(n, m, repetitions);

        const long long totalCells =
            1LL * n * n;

        const double ratio =
            static_cast<double>(m) /
            static_cast<double>(totalCells);

        std::ofstream fout("output.txt");

        if (!fout.is_open())
        {
            throw std::runtime_error(
                "Cannot create output.txt"
            );
        }

        fout << std::fixed
             << std::setprecision(3);

        fout
            << "For n = " << n
            << ", m = " << m
            << ", m/n^2 = " << ratio
            << '\n';

        fout
            << "Mean free-zone size = "
            << statistics.mean
            << '\n';

        fout
            << "Median free-zone size = "
            << statistics.median
            << "\n\n";

        const std::vector<StudyPoint> study =
            studyDependence(n, repetitions);

        fout << "Dependence on m/n^2:\n";

        fout
            << std::setw(10) << "m/n^2"
            << std::setw(10) << "m"
            << std::setw(18) << "mean"
            << std::setw(18) << "median"
            << '\n';

        for (const StudyPoint& point : study)
        {
            fout
                << std::setw(10)
                << point.ratio

                << std::setw(10)
                << point.m

                << std::setw(18)
                << point.meanFree

                << std::setw(18)
                << point.medianFree

                << '\n';
        }

        fout.close();

        dataSets.erase(dataSets.begin());

        if (dataSets.empty())
        {
            dataSets = askForNewInputData();
        }

        writeInputData("input.txt", dataSets);

        std::cout
            << "Calculation completed.\n"
            << "Results written to output.txt\n"
            << "Used input: n = " << n
            << ", m = " << m
            << ", repetitions = " << repetitions
            << '\n';

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
