#include "Ranking.h"
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <sstream>

// Construtor padr�o
Ranking::Ranking() {}

// Fun��o para adicionar um registro ao ranking
void Ranking::addRecord(const std::string& playerName, unsigned int score, unsigned int game_time, const std::string& date) {
    Record newRecord = { playerName, score, game_time, date };
    records.push_back(newRecord);
    sortRanking();
}

// Fun��o para ordenar o ranking pela pontua��o (decrescente)
void Ranking::sortRanking() {
    std::sort(records.begin(), records.end(), [](const Record& a, const Record& b) {
        return a.score > b.score; // Maior pontua��o primeiro
        });
}

// Fun��o para salvar os registros no arquivo ranking.csv
void Ranking::saveToFile(const std::string& filename) {
    std::ofstream outFile(filename, std::ios::app);
    if (!outFile.is_open()) {
        return; // Retorna se o arquivo n�o puder ser aberto
    }

    // Verifica se o arquivo est� vazio (ou seja, foi rec�m-criado)
    outFile.seekp(0, std::ios::end); // Move o ponteiro para o final do arquivo
    int fileSize = outFile.tellp(); // Obt�m a posi��o do ponteiro (tamanho do arquivo)
    if (fileSize == 0) {
        outFile << "Nome, Pontuacao, Tempo, Data\n"; // Cabe�alho do arquivo
    }

    for (const Record& record : records) {
        outFile << record.playerName << ", "
            << record.score << ", "
            << record.game_time << ", "
            << record.date << "\n";
    }

    outFile.close();
}
