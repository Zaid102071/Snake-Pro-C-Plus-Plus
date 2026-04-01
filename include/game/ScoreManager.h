#pragma once

#include "game/Config.h"
#include <string>
#include <vector>

namespace snakepro {

struct HighScoreEntry {
    int score;
    std::string date;
};

class ScoreManager {
public:
    ScoreManager();
    
    void loadScores();
    void saveScores() const;
    void addScore(int score);
    
    int getHighScore() const;
    const std::vector<HighScoreEntry>& getScores() const { return scores_; }
    
private:
    std::vector<HighScoreEntry> scores_;
    std::string getFilePath() const;
};

}
