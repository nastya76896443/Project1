#pragma once

#include <compare>

struct Cell
{
    int row{};
    int col{};

    auto operator<=>(const Cell&) const = default;
};
