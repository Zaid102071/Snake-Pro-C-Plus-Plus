#pragma once

#include <utility>

namespace snakepro {

using Position = std::pair<int, int>;

inline bool operator==(const Position& a, const Position& b) {
    return a.first == b.first && a.second == b.second;
}

inline bool operator!=(const Position& a, const Position& b) {
    return !(a == b);
}

}
