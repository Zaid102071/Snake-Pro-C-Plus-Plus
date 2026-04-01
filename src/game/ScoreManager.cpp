#include "game/ScoreManager.h"
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <ctime>

namespace snakepro {

ScoreManager::ScoreManager() {
    loadScores();
}

std::string ScoreManager::getFilePath() const {
    std::filesystem::path path = std::filesystem::current_path() / Settings::kHighScoreFile;
    return path.string();
}

void ScoreManager::loadScores() {
    std::ifstream file(getFilePath(), std::ios::binary);
    if (!file.is_open()) return;

    int count;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));

    scores_.clear();
    for (int i = 0; i < count && i < 10; ++i) {
        HighScoreEntry entry;
        int nameLen, dateLen;
        file.read(reinterpret_cast<char*>(&entry.score), sizeof(entry.score));
        file.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        entry.name.resize(nameLen);
        file.read(&entry.name[0], nameLen);
        file.read(reinterpret_cast<char*>(&dateLen), sizeof(dateLen));
        entry.date.resize(dateLen);
        file.read(&entry.date[0], dateLen);
        scores_.push_back(entry);
    }
}

void ScoreManager::saveScores() const {
    std::ofstream file(getFilePath(), std::ios::binary);
    if (!file.is_open()) return;

    int count = std::min(static_cast<int>(scores_.size()), 10);
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (int i = 0; i < count; ++i) {
        file.write(reinterpret_cast<const char*>(&scores_[i].score), sizeof(scores_[i].score));
        int nameLen = static_cast<int>(scores_[i].name.size());
        file.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
        file.write(scores_[i].name.data(), nameLen);
        int dateLen = static_cast<int>(scores_[i].date.size());
        file.write(reinterpret_cast<const char*>(&dateLen), sizeof(dateLen));
        file.write(scores_[i].date.data(), dateLen);
    }
}

void ScoreManager::addScore(int score) {
    time_t now = time(nullptr);
    char buf[64];
    struct tm* timeinfo = localtime(&now);
    if (timeinfo) {
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", timeinfo);
    } else {
        buf[0] = '\0';
    }

    scores_.push_back({score, std::string(buf), "Player"});
    std::sort(scores_.begin(), scores_.end(),
              [](const HighScoreEntry& a, const HighScoreEntry& b) {
                  return a.score > b.score;
              });

    if (scores_.size() > 10) {
        scores_.resize(10);
    }

    saveScores();
}

int ScoreManager::getHighScore() const {
    return scores_.empty() ? 0 : scores_.front().score;
}

}
