#pragma once

#include <vector>

struct Statistics
{
    double mean{};
    double median{};
};

struct StudyPoint
{
    double ratio{};          // m / n^2
    int m{};
    double meanFree{};
    double medianFree{};
};

int experiment(int n, int m);
Statistics runExperiments(int n, int m, int repetitions);
std::vector<StudyPoint> studyDependence(int n, int repetitions);
