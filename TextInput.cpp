#include "TextInput.h"
#include <SFML/Graphics.hpp>
#include <iostream>

std::string getPlayerName(sf::RenderWindow& window) {
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Erro ao carregar a fonte!" << std::endl;
        return "";
    }

    sf::Text prompt("Digite seu nome:", font, 24);
    prompt.setPosition(10, 10);
    prompt.setFillColor(sf::Color::White);

    sf::Text nameText("", font, 24);
    nameText.setPosition(10, 50);
    nameText.setFillColor(sf::Color::Yellow);

    std::string playerName;
    sf::Event event;

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return "";
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b') { // Backspace
                    if (!playerName.empty()) {
                        playerName.pop_back();
                    }
                }
                else if (event.text.unicode == '\r') { // Enter
                    if (!playerName.empty()) {
                        return playerName; // Retorna o nome digitado
                    }
                }
                else if (event.text.unicode < 128) { // Caracteres válidos
                    playerName += static_cast<char>(event.text.unicode);
                }
            }
        }

        nameText.setString(playerName);

        window.clear();
        window.draw(prompt);
        window.draw(nameText);
        window.display();
    }

    return ""; // Caso a janela seja fechada
}
