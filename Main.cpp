#include "Menu.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#define CELL_SIZE 32
#define MAP_WIDTH 28
#define MAP_HEIGHT 31
#define SCREEN_RESIZE 1
#define FONT_HEIGHT 10

int main()
{
    //sf::Music music;
    //music.openFromFile(("C:/Users/inaci/source/repos/pacmanSFML/pacmanSFML/Digital-Zenith.wav"));
    //music.setLoop(true);
    //music.play();
    //Track: Digital Zenith
    //Fonte: https://www.fiftysounds.com
    sf::RenderWindow window(
        sf::VideoMode(CELL_SIZE * MAP_WIDTH * SCREEN_RESIZE,
            (FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT) * SCREEN_RESIZE),
        "Pac-Man", sf::Style::Close);

    window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * MAP_WIDTH, FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT)));
    Menu menu(window);
    menu.run();

    return 0;
}