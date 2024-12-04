#include "Menu.h"
#include "Dificulty.h" 
#include "Sobre.h"
#include "Ranking.h"
#include "ReadRanking.h"

#include <cstdlib> // Para usar exit()

// Variavel global para controle do modo de desafio escuro
bool dark_mode = false;

Menu::Menu(sf::RenderWindow& i_window) : window(i_window), selectedItemIndex(0)
{
    // Seleciona a fonte para ser usada
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) 
    {
        exit(1); //Tratamento de erro
    }
    createMenuItems(); // cria as opçoes do menu
}

void Menu::createMenuItems()
{
    const std::vector<std::string> itemTexts = { "Jogar - Classico", "Modo Desafio", "Ranking", "Sobre", "Sair" }; //indexa os valores na string 

    for (const std::string& text : itemTexts) {
        sf::Text menuItem(text, font, 30); //tamanho da fonte 
        sf::FloatRect textRect = menuItem.getLocalBounds(); //centralizar o sprite
        menuItem.setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);
        menuItems.push_back(menuItem);
    }

    positionMenuItems();
}

void Menu::positionMenuItems()
{
    float totalHeight = menuItems.size() * 40.f; // Calcula a altura total do menu
    for (size_t i = 0; i < menuItems.size(); ++i) {
        menuItems[i].setPosition(window.getSize().x / 2.f,
            window.getSize().y / 2.f + i * 40.f - totalHeight / 2.f);
    }
}

void Menu::run()
{
    //inicia um evento 
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
                else if (event.key.code == sf::Keyboard::Return) { // Enter key
                    switch (selectedItemIndex)
                    {
                    case 0: // Jogar classico
                    {
                        Dificulty dificuldade(window); // Instancia a tela de dificuldade
                        dificuldade.run();
                        break;
                    }
                    case 1: // Jogar desafio
                    {
                        dark_mode = !dark_mode;
                        Dificulty dificuldade(window); // Instancia a tela de dificuldade
                        dificuldade.run();
                        break;
                    }
                    case 2: {// Ranking
                        ReadRanking rankingScreen(window);
                        rankingScreen.run();
                        break;
                    }
                    case 3: // Sobre
                    {
                        Sobre sobre(window);
                        sobre.run();
                        break;
                    }
                    case 4: // Sair
                        window.close();
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

void Menu::draw()
{

    //desenha o menu na tela e se selecionado mostra-o em verde 
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

void Menu::moveUp()
{
    if (selectedItemIndex - 1 >= 0) {
        selectedItemIndex--;
    }
}

void Menu::moveDown()
{
    if (selectedItemIndex + 1 < menuItems.size()) {
        selectedItemIndex++;
    }
}