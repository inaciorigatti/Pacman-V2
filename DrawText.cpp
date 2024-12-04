#include <cmath>
#include <SFML/Graphics.hpp>

#include "DrawText.h"
#include "Global.h"

// Função para desenhar texto na tela
void draw_text(bool i_center, unsigned short i_x, unsigned short i_y, const std::string& i_text, sf::RenderWindow& i_window)
{
    short character_x = i_x; // Posição x inicial do caractere
    short character_y = i_y; // Posição y inicial do caractere

    unsigned char character_width; // Largura de cada caractere

    sf::Sprite character_sprite; // Sprite para desenhar o caractere

    sf::Texture font_texture; // Textura da fonte
    font_texture.loadFromFile("C:/Users/inaci/source/repos/pacmanSFML/pacmanSFML/Font.png"); // Carrega a textura da fonte

    // Calcula a largura de cada caractere based on the provided font file.
    character_width = font_texture.getSize().x / 96;

    character_sprite.setTexture(font_texture);

    // Centraliza o texto se i_center for verdadeiro
    if (1 == i_center) {
        // Calcula a posição x e y para centralizar o texto na primeira linha.
        character_x = static_cast<short>(round(0.5f * (CELL_SIZE * MAP_WIDTH - character_width * i_text.substr(0, i_text.find_first_of('\n')).size())));
        character_y = static_cast<short>(round(0.5f * (CELL_SIZE * MAP_HEIGHT - FONT_HEIGHT * (1 + std::count(i_text.begin(), i_text.end(), '\n')))));
    }

    // Itera por cada caractere do texto
    for (std::string::const_iterator a = i_text.begin(); a != i_text.end(); a++) {
        if ('\n' == *a) { // Se o caractere for uma quebra de linha
            if (1 == i_center) {
                // Calcula o centro para a próxima linha.  A lógica substr extrai a próxima linha  e calcula seu comprimento.
                character_x = static_cast<short>(round(0.5f * (CELL_SIZE * MAP_WIDTH - character_width * i_text.substr(1 + a - i_text.begin(), i_text.find_first_of('\n', 1 + a - i_text.begin()) - (1 + a - i_text.begin())).size())));
            }
            else {
                character_x = i_x; // Volta para a posição x inicial se não estiver centralizado
            }

            character_y += FONT_HEIGHT; // Avança para a próxima linha

            continue; // Pula para o próximo caractere
        }

        character_sprite.setPosition(character_x, character_y); // Define a posição do sprite do caractere
        // Define a região da textura para o caractere atual (considerando que o espaço é o caractere 32)
        character_sprite.setTextureRect(sf::IntRect(character_width * (*a - 32), 0, character_width, FONT_HEIGHT));

        character_x += character_width; // Avança para o próximo caractere

        i_window.draw(character_sprite); // Desenha o caractere
    }
}