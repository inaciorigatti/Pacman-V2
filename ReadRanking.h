#ifndef READRANKING_H
#define READRANKING_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "Global.h" // Certifique-se de que Global.h esteja inclu�do aqui ou onde for necess�rio

class ReadRanking {
public:
    ReadRanking(sf::RenderWindow& window);
    void run();

private:
    sf::RenderWindow& window;
    sf::Font font;
    std::vector<std::string> rankingLines;

    void loadRanking(const std::string& filename);
    void drawRanking();
};

#endif