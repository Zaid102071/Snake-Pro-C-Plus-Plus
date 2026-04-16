#include "ScoreManager.hpp"
#include <fstream>
#include <algorithm>

namespace snake {

ScoreManager::ScoreManager()
    : score_(0), high_score_(0) {
    load_high_score();
}

void ScoreManager::add(std::uint32_t points) {
    score_ = std::min(score_ + points, MAX_SCORE);
    if (score_ > high_score_) {
        high_score_ = score_;
    }
}

void ScoreManager::reset() {
    score_ = 0;
}

void ScoreManager::save_high_score() {
    std::ofstream file(HIGH_SCORE_FILE, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(&high_score_), sizeof(high_score_));
    }
}

void ScoreManager::load_high_score() {
    std::ifstream file(HIGH_SCORE_FILE, std::ios::binary);
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(&high_score_), sizeof(high_score_));
        if (!file) high_score_ = 0;
    }
}

std::uint32_t ScoreManager::score() const noexcept {
    return score_;
}

std::uint32_t ScoreManager::high_score() const noexcept {
    return high_score_;
}

}
