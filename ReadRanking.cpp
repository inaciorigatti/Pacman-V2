#include "ReadRanking.h"
#include <iostream> // Para debugging


ReadRanking::ReadRanking(sf::RenderWindow& i_window) : window(i_window)
{
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        // mostrar alguma coisa
    }
    loadRanking("ranking.csv");
}

void ReadRanking::loadRanking(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        rankingLines.push_back("Erro ao abrir o arquivo de ranking.");
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        rankingLines.push_back(line);
    }
    inFile.close();


    if (rankingLines.empty())
    {
        rankingLines.push_back("Ranking Vazio");
    }
}


void ReadRanking::drawRanking() {
    int line_y = 0;


    for (const std::string& line : rankingLines) {
        sf::Text rankingText(line, font, 20);
        rankingText.setFillColor(sf::Color::White);
        rankingText.setPosition(10, line_y);
        window.draw(rankingText);
        line_y += 35;  // Ajuste o espaçamento entre as linhas conforme necessário

    }
}

void ReadRanking::run() {
    sf::Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) // Pressione ESC para voltar
                {
                    return;  // Retorna ao menu
                }
            }
        }

        window.clear();
        drawRanking();
        window.display();
    }
}