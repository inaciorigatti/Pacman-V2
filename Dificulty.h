#ifndef DIFICULTY_H
#define DIFICULTY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Dificulty
{
private:
    sf::RenderWindow& window;
    int selectedItemIndex;
    sf::Font font;
    std::vector<sf::Text> menuItems;

    void createMenuItems();
    void positionMenuItems();
    void draw();
    void moveUp();
    void moveDown();

public:
    Dificulty(sf::RenderWindow& i_window);
    void run();
    int getSelectedDifficulty() const;  // Para retornar a dificuldade escolhida
};

#endif // DIFICULTY_H
