#include <array>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Global.h"
#include "Pacman.h"
#include "MapCollision.h"

// Construtor da classe Pacman, inicializando os atributos com valores padrão.
Pacman::Pacman() :
    animation_over(0), // Indica se a animação terminou (0 = não terminou).
    dead(0), // Indica se o Pacman está morto (0 = vivo).
    direction(0), // Direção inicial do Pacman (0 = direita).
    energizer_timer(0), // Temporizador do efeito de energizador.
    position({ 0, 0 }), // Posição inicial do Pacman.
    score(0), // Pontuação inicial.
    lives(3), // Número inicial de vidas.
    move_counter(0),
    key_pressed{ false, false, false, false },
    cherryActive(false), 
    cherryTimer(0)
{
}

unsigned int Pacman::get_move_counter() {
    return move_counter; // Retorna o valor do contador de movimentos.
}

// Método para obter o número de vidas restantes.
unsigned int Pacman::get_lives() {
    return lives;
}

// Método para reduzir uma vida do Pacman.
void Pacman::lose_life() {
    if (lives > 0) {
        lives--; // Reduz uma vida.
        animation_timer = 0; // Reinicia o temporizador da animação de morte.
    }
}

// Método para obter a pontuação atual.
unsigned int Pacman::get_score() {
    return score;
}

// Verifica se a animação de morte ou vitória terminou.
bool Pacman::get_animation_over() {
    return animation_over;
}

// Verifica se o Pacman está morto.
bool Pacman::get_dead() {
    return dead;
}

// Obtém a direção atual do Pacman.
unsigned char Pacman::get_direction() {
    return direction;
}

// Retorna o tempo restante do efeito de energizador.
unsigned short Pacman::get_energizer_timer() {
    return energizer_timer;
}

// Método para desenhar o Pacman ou sua animação de morte.
void Pacman::draw(bool i_victory, sf::RenderWindow& i_window) {
    unsigned char frame = static_cast<unsigned char>(floor(animation_timer / static_cast<float>(PACMAN_ANIMATION_SPEED)));
    sf::Sprite sprite; // Sprite do Pacman.
    sf::Texture texture; // Textura a ser carregada.

    // Define a posição do sprite no jogo.
    sprite.setPosition(position.x, position.y);

    // Caso Pacman esteja morto ou tenha vencido.
    if (1 == dead || 1 == i_victory) {
        texture.loadFromFile("C:/Users/inaci/source/repos/pacmanSFML/pacmanSFML/PacmanDeath" + std::to_string(CELL_SIZE) + ".png");
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(CELL_SIZE * frame, 0, CELL_SIZE, CELL_SIZE));
        i_window.draw(sprite);

        animation_over = 1; // Marca que a animação terminou.
        dead = 0; // Define que o Pacman não está mais "morto".
    }
    // Caso contrário, desenha o Pacman normal.
    else {
        texture.loadFromFile("C:/Users/inaci/source/repos/pacmanSFML/pacmanSFML/Pacman" + std::to_string(CELL_SIZE) + ".png");
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(CELL_SIZE * frame, CELL_SIZE * direction, CELL_SIZE, CELL_SIZE));
        i_window.draw(sprite);

        // Atualiza o temporizador de animação.
        animation_timer = (1 + animation_timer) % (PACMAN_ANIMATION_FRAMES * PACMAN_ANIMATION_SPEED);
    }
}

// Método para reiniciar o estado do Pacman.
void Pacman::reset() {
    animation_over = 0;
    dead = 0;
    direction = 0;
    animation_timer = 0;
    energizer_timer = 0;
    score = 0; // Pontuação zerada.
    lives = 3; // Reinicia com 3 vidas.
    move_counter = 0;
}

// Define o valor do temporizador da animação.
void Pacman::set_animation_timer(unsigned short i_animation_timer) {
    animation_timer = i_animation_timer;
}

// Define se o Pacman está morto e reinicia a animação se necessário.
void Pacman::set_dead(bool i_dead) {
    dead = i_dead;
    if (1 == dead) {
        animation_timer = 0; // Reinicia o temporizador.
    }
}

// Define a posição do Pacman no mapa.
void Pacman::set_position(short i_x, short i_y) {
    position = { i_x, i_y };
}

// Atualiza o estado do Pacman, incluindo movimento e coleta de itens.
void Pacman::update(unsigned char i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map) {
    std::array<bool, 4> walls{}; // Verifica colisões em cada direção.
    walls[0] = map_collision(0, 0, PACMAN_SPEED + position.x, position.y, i_map); // Direita.
    walls[1] = map_collision(0, 0, position.x, position.y - PACMAN_SPEED, i_map); // Cima.
    walls[2] = map_collision(0, 0, position.x - PACMAN_SPEED, position.y, i_map); // Esquerda.
    walls[3] = map_collision(0, 0, position.x, PACMAN_SPEED + position.y, i_map); // Baixo.

    // Detecta teclas pressionadas para registrar apenas o primeiro pressionamento.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        if (!key_pressed[0]) { // Só conta se ainda não foi pressionada.
            move_counter++;
            key_pressed[0] = true;
        }
        if (0 == walls[0]) {
            direction = 0; // Direção direita.
        }
    }
    else {
        key_pressed[0] = false; // Libera para o próximo pressionamento.
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        if (!key_pressed[1]) {
            move_counter++;
            key_pressed[1] = true;
        }
        if (0 == walls[1]) {
            direction = 1; // Direção cima.
        }
    }
    else {
        key_pressed[1] = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        if (!key_pressed[2]) {
            move_counter++;
            key_pressed[2] = true;
        }
        if (0 == walls[2]) {
            direction = 2; // Direção esquerda.
        }
    }
    else {
        key_pressed[2] = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        if (!key_pressed[3]) {
            move_counter++;
            key_pressed[3] = true;
        }
        if (0 == walls[3]) {
            direction = 3; // Direção baixo.
        }
    }
    else {
        key_pressed[3] = false;
    }

    // Movimenta o Pacman se não houver parede na direção escolhida.
    if (0 == walls[direction]) {
        switch (direction) {
        case 0: position.x += PACMAN_SPEED; break; // Movimenta para direita.
        case 1: position.y -= PACMAN_SPEED; break; // Movimenta para cima.
        case 2: position.x -= PACMAN_SPEED; break; // Movimenta para esquerda.
        case 3: position.y += PACMAN_SPEED; break; // Movimenta para baixo.
        }
    }

    // Teletransporte horizontal caso ultrapasse as bordas.
    if (-CELL_SIZE >= position.x) {
        position.x = CELL_SIZE * MAP_WIDTH - PACMAN_SPEED;
    }
    else if (CELL_SIZE * MAP_WIDTH <= position.x) {
        position.x = PACMAN_SPEED - CELL_SIZE;
    }

    // Coleta de Pellet.
    if (i_map[position.x / CELL_SIZE][position.y / CELL_SIZE] == Cell::Pellet) {
        score += (cherryActive) ? 20 : 10; // Double points while active
        i_map[position.x / CELL_SIZE][position.y / CELL_SIZE] = Cell::Empty;
    }

    // Coleta de Energizer.
    if (i_map[position.x / CELL_SIZE][position.y / CELL_SIZE] == Cell::Energizer) {
        score += 10; // Adiciona pontos.
        energizer_timer = static_cast<unsigned short>(ENERGIZER_DURATION / pow(2, i_level)); // Define duração do efeito.
        i_map[position.x / CELL_SIZE][position.y / CELL_SIZE] = Cell::Empty; // Remove o Energizer.
    }
    else {
        energizer_timer = std::max(0, energizer_timer - 1); // Diminui o tempo do efeito.
    }

    if (i_map[position.x / CELL_SIZE][position.y / CELL_SIZE] == Cell::Cherry) {
        score += 20; // Double points!
        cherryActive = true;
        cherryTimer = CHERRY_DURATION * 60; // Timer in frames (assuming 60fps)
        i_map[position.x / CELL_SIZE][position.y / CELL_SIZE] = Cell::Empty;
    }

    if (cherryActive) {
        cherryTimer--;
        if (cherryTimer <= 0) {
            cherryActive = false;
        }
    }


}

// Obtém a posição atual do Pacman.
Position Pacman::get_position() {
    return position;
}
