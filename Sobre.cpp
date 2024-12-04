
#include "Sobre.h"

Sobre::Sobre(sf::RenderWindow& i_window) : window(i_window)
{
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) // Substitua pelo caminho correto no seu sistema
    {
        exit(1); // Saida simples para caso de erro
    }

    // Configura o titulo
    setupText(title, "Sobre Pac-Man", window.getSize().x / 2.f, 50.f, 40);

    // Configura o conte�do
    std::string sobreText =
        "Pac-Man e um dos jogos mais iconicos da historia dos videogames.\n"
        "Criado em 1980 pela empresa Namco, o objetivo do jogo e guiar o Pac-Man\n"
        "atraves de um labirinto, comendo pallets e evitando fantasmas.\n\n"
        "Pac-Man e reconhecido globalmente e continua sendo amado por\n"
        "jogadores de todas as idades.\n\n\n\n\n\n"
        "Desenvolvedores: Inácio Rigatti, Lucas Coelho e Nilson Neto. Novembro/2024.\n\n"
        "Link Github do projeto: https://github.com/inaciorigatti/Pac-Man\n\n";



    setupText(content, sobreText, window.getSize().x / 2.f, window.getSize().y / 2.f - 50.f, 20);

    // Configura a opcao de voltar
    setupText(backOption, "Pressione ESC para voltar", window.getSize().x / 2.f, window.getSize().y - 50.f, 20);
}

void Sobre::setupText(sf::Text& text, const std::string& str, float x, float y, unsigned int size)
{
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);
    text.setPosition(x, y);
}

void Sobre::run()
{
    sf::Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                return; // Volta ao menu principal
            }
        }

        window.clear(sf::Color::Black);
        window.draw(title);
        window.draw(content);
        window.draw(backOption);
        window.display();
    }
}