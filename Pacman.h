#pragma once

class Pacman
{
private:
    // Atributos privados
    bool animation_over; // Anima��o de morte
    bool dead;           // Indica se o Pacman est� morto
    unsigned int score;  // Pontua��o
    unsigned int lives;  // N�mero de vidas
    unsigned int move_counter; // Contador de movimentos

    unsigned short animation_timer;  // Temporizador da anima��o
    unsigned short energizer_timer;  // Temporizador do energizador
    bool key_pressed[4];
    // Dire��o e posi��o do Pacman
    unsigned char direction;
    Position position;

    bool cherryActive; 
    unsigned short cherryTimer;

public:
    // Construtor
    Pacman();

    // M�todos p�blicos
    bool get_animation_over();
    bool get_dead();
    unsigned int get_score();
    unsigned int get_lives();
    unsigned int get_move_counter(); // Declara��o correta
    unsigned char get_direction();
    unsigned short get_energizer_timer();

    void draw(bool i_victory, sf::RenderWindow& i_window);
    void lose_life();
    void reset_position();
    void reset();
    void set_animation_timer(unsigned short i_animation_timer);
    void set_dead(bool i_dead);
    void set_position(short i_x, short i_y);
    void update(unsigned char i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map);

    Position get_position();
};
