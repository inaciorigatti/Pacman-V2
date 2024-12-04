#ifndef RANKING_H
#define RANKING_H

#include <string>
#include <vector>

class Ranking {
private:
    struct Record {
        std::string playerName;
        unsigned int score;
        unsigned int game_time;
        std::string date;
    };

    std::vector<Record> records;

    // Ordena o ranking por pontuação de forma decrescente
    void sortRanking();

public:
    Ranking();

    // Adiciona um novo registro ao ranking
    void addRecord(const std::string& playerName, unsigned int score, unsigned int game_time, const std::string& date);

    // Salva o ranking no arquivo ranking.csv
    void saveToFile(const std::string& filename = "ranking.csv");
};

#endif // RANKING_H
