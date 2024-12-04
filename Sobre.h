#ifndef SOBRE_H
#define SOBRE_H

#include <SFML/Graphics.hpp>
#include <string>

class Sobre
{
private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text title;
    sf::Text content;
    sf::Text backOption;

    void setupText(sf::Text& text, const std::string& str, float x, float y, unsigned int size);

public:
    Sobre(sf::RenderWindow& i_window);
    void run();
};

#endif 