#include "Dificulty.h"
#include "Easy.h"
#include "Medium.h"
#include "Hard.h"

#include <cstdlib> // Para usar exit()

Dificulty::Dificulty(sf::RenderWindow& i_window) : window(i_window), selectedItemIndex(0)
{
    // Seleciona a fonte para ser usada
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        exit(1); // Tratamento de erro
    }
    createMenuItems(); // Cria as opções de dificuldade
}

void Dificulty::createMenuItems()
{
    const std::vector<std::string> itemTexts = { "Facil", "Medio", "Dificil", "Voltar"}; // Opções de dificuldade

    for (const std::string& text : itemTexts) {
        sf::Text menuItem(text, font, 30); // Tamanho da fonte
        sf::FloatRect textRect = menuItem.getLocalBounds(); // Centraliza o texto
        menuItem.setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);
        menuItems.push_back(menuItem);
    }

    positionMenuItems();
}

void Dificulty::positionMenuItems()
{
    float totalHeight = menuItems.size() * 40.f; // Calcula a altura total do menu
    for (size_t i = 0; i < menuItems.size(); ++i) {
        menuItems[i].setPosition(window.getSize().x / 2.f,
            window.getSize().y / 2.f + i * 40.f - totalHeight / 2.f);
    }
}

void Dificulty::run()
{
    // Inicia um evento
    sf::Event event;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    moveUp();
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    moveDown();
                }
                else if (event.key.code == sf::Keyboard::Return) { // Tecla Enter
                    // Trata a lógica de acordo com a dificuldade selecionada
                    switch (selectedItemIndex)
                    {
                    case 0: 
                    {// Fácil
                        
                        Easy easy; 
                        easy.run();
                        break;
                    }

                    case 1: 
                    { // Médio
                        Medium medium;
                        medium.run();
                        break;
                    }

                    case 2:
                    {// Difícil
                        Hard hard;
                        hard.run();
                        break;
                    }
                    

                    case 3:
                        return;

                    default:
                        break;
                    }
                }
            }
        }

        window.clear();
        draw();
        window.display();
    }
}

void Dificulty::draw()
{
    // Desenha o menu na tela e se selecionado mostra-o em verde
    for (size_t i = 0; i < menuItems.size(); ++i) {
        if (i == selectedItemIndex) {
            menuItems[i].setFillColor(sf::Color::Green);
        }
        else {
            menuItems[i].setFillColor(sf::Color::White);
        }
        window.draw(menuItems[i]);
    }
}

void Dificulty::moveUp()
{
    if (selectedItemIndex - 1 >= 0) {
        selectedItemIndex--;
    }
}

void Dificulty::moveDown()
{
    if (selectedItemIndex + 1 < menuItems.size()) {
        selectedItemIndex++;
    }
}

int Dificulty::getSelectedDifficulty() const
{
    // Retorna a dificuldade escolhida (0 = Fácil, 1 = Médio, 2 = Difícil)
    return selectedItemIndex;
}
