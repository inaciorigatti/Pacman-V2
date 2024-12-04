#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu
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
    Menu(sf::RenderWindow& i_window);
    void run();
};

#endif // MENU_H