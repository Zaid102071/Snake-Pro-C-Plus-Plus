#pragma once

#include <cstdint>
#include <string>

namespace snake {

class ScoreManager {
public:
    ScoreManager();

    void add(std::uint32_t points);
    void reset();
    void save_high_score();
    void load_high_score();

    [[nodiscard]] std::uint32_t score() const noexcept;
    [[nodiscard]] std::uint32_t high_score() const noexcept;

private:
    std::uint32_t score_;
    std::uint32_t high_score_;
    static constexpr const char* HIGH_SCORE_FILE = "highscore.dat";
};

}
