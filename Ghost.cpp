#include <array>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Global.h"
#include "Pacman.h"
#include "Ghost.h"
#include "MapCollision.h"

// Construtor da classe Ghost, recebe o ID do fantasma
Ghost::Ghost(unsigned char i_id) :
    id(i_id)
{

}

// Verifica colis�o entre o fantasma e o Pacman
bool Ghost::pacman_collision(const Position& i_pacman_position)
{
    // Verifica se as coordenadas x e y do fantasma e do Pacman est�o dentro do mesmo bloco
    // CELL_SIZE representa o tamanho de um bloco no mapa
    if (position.x > i_pacman_position.x - CELL_SIZE && position.x < CELL_SIZE + i_pacman_position.x) {
        if (position.y > i_pacman_position.y - CELL_SIZE && position.y < CELL_SIZE + i_pacman_position.y) {
            return 1; // Colis�o detectada
        }
    }
    return 0; // Sem colis�o
}

// Calcula a dist�ncia entre o fantasma e o alvo, considerando uma dire��o de movimento
float Ghost::get_target_distance(unsigned char i_direction)
{
    short x = position.x;
    short y = position.y;

    // Simula o movimento do fantasma na dire��o especificada
    switch (i_direction)
    {
    case 0: // Direita
    {
        x += GHOST_SPEED;
        break;
    }
    case 1: // Cima
    {
        y -= GHOST_SPEED;
        break;
    }
    case 2: // Esquerda
    {
        x -= GHOST_SPEED;
        break;
    }
    case 3: // Baixo
    {
        y += GHOST_SPEED;
    }
    }

    // Calcula a dist�ncia usando o teorema de Pit�goras
    return static_cast<float>(sqrt(pow(x - target.x, 2) + pow(y - target.y, 2)));
}

// Desenha o fantasma na janela
void Ghost::draw(bool i_flash, sf::RenderWindow& i_window)
{
    // Calcula o frame atual da anima��o
    unsigned char body_frame = static_cast<unsigned char>(floor(animation_timer / static_cast<float>(GHOST_ANIMATION_SPEED)));

    sf::Sprite body; // Sprite para o corpo do fantasma
    sf::Sprite face; // Sprite para a face do fantasma

    sf::Texture texture;
    texture.loadFromFile("C:/Users/inaci/source/repos/pacmanSFML/pacmanSFML/Ghost" + std::to_string(CELL_SIZE) + ".png"); // Carrega a textura do fantasma

    body.setTexture(texture);
    body.setPosition(position.x, position.y);
    // Define a regi�o da textura para o frame atual da anima��o
    body.setTextureRect(sf::IntRect(CELL_SIZE * body_frame, 0, CELL_SIZE, CELL_SIZE));

    face.setTexture(texture);
    face.setPosition(position.x, position.y);

    // Define a cor e a face do fantasma dependendo do modo (normal, assustado, fugindo)
    if (0 == frightened_mode) // Modo normal
    {
        // Define a cor do fantasma de acordo com seu ID
        switch (id)
        {
        case 0: // Vermelho
        {
            body.setColor(sf::Color(255, 0, 0));
            break;
        }
        case 1: // Rosa
        {
            body.setColor(sf::Color(255, 182, 255));
            break;
        }
        case 2: // Ciano
        {
            body.setColor(sf::Color(0, 255, 255));
            break;
        }
        case 3: // Laranja
        {
            body.setColor(sf::Color(255, 182, 85));
        }
        }

        face.setTextureRect(sf::IntRect(CELL_SIZE * direction, CELL_SIZE, CELL_SIZE, CELL_SIZE)); // Define a face de acordo com a dire��o

        i_window.draw(body); // Desenha o corpo
    }
    else if (1 == frightened_mode) // Modo assustado
    {
        // Define a cor do corpo e da face, com efeito de piscar
        if (1 == i_flash && 0 == body_frame % 2)
        {
            body.setColor(sf::Color(255, 255, 255));
            face.setColor(sf::Color(255, 0, 0));
        }
        else
        {
            body.setColor(sf::Color(36, 36, 255));
            face.setColor(sf::Color(255, 255, 255));
        }

        face.setTextureRect(sf::IntRect(4 * CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE)); // Face de assustado

        i_window.draw(body); // Desenha o corpo
    }
    else // Modo fugindo (ap�s ser comido)
    {
        face.setTextureRect(sf::IntRect(CELL_SIZE * direction, 2 * CELL_SIZE, CELL_SIZE, CELL_SIZE)); // Face de fugindo
    }

    i_window.draw(face); // Desenha a face

    // Atualiza o timer da anima��o, evitando overflow
    animation_timer = (1 + animation_timer) % (GHOST_ANIMATION_FRAMES * GHOST_ANIMATION_SPEED);
}

// Reinicia o fantasma para sua posi��o inicial
void Ghost::reset(const Position& i_home, const Position& i_home_exit)
{
    movement_mode = 0;
    // Todos os fantasmas, exceto o vermelho, podem usar a porta
    use_door = 0 < id;

    direction = 0;
    frightened_mode = 0;
    frightened_speed_timer = 0;

    animation_timer = 0;

    home = i_home;
    home_exit = i_home_exit;
    target = i_home_exit;
}


// Define a posi��o do fantasma
void Ghost::set_position(short i_x, short i_y)
{
    position = { i_x, i_y };
}

// Alterna entre os modos de movimento (scatter e chase)
void Ghost::switch_mode()
{
    movement_mode = 1 - movement_mode;
}


// Atualiza o estado do fantasma
void Ghost::update(unsigned char i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, Ghost& i_ghost_0, Pacman& i_pacman)
{
    bool move = 0; // Indica se o fantasma pode se mover
    unsigned char available_ways = 0; // N�mero de caminhos dispon�veis
    unsigned char speed = GHOST_SPEED; // Velocidade do fantasma
    std::array<bool, 4> walls{}; // Indica se h� paredes em cada dire��o


    // L�gica para alternar entre os modos assustado e normal
    if (0 == frightened_mode && i_pacman.get_energizer_timer() == ENERGIZER_DURATION / pow(2, i_level)) {
        frightened_speed_timer = GHOST_FRIGHTENED_SPEED;
        frightened_mode = 1;
    }
    else if (0 == i_pacman.get_energizer_timer() && 1 == frightened_mode) {
        frightened_mode = 0;
    }


    // Define a velocidade de fuga quando o fantasma � comido
    if (2 == frightened_mode && 0 == position.x % GHOST_ESCAPE_SPEED && 0 == position.y % GHOST_ESCAPE_SPEED) {
        speed = GHOST_ESCAPE_SPEED;
    }

    update_target(i_pacman.get_direction(), i_ghost_0.get_position(), i_pacman.get_position()); // Atualiza o alvo do fantasma


    // Verifica colis�es com paredes em cada dire��o
    walls[0] = map_collision(0, use_door, speed + position.x, position.y, i_map); // Direita
    walls[1] = map_collision(0, use_door, position.x, position.y - speed, i_map); // Cima
    walls[2] = map_collision(0, use_door, position.x - speed, position.y, i_map); // Esquerda
    walls[3] = map_collision(0, use_door, position.x, speed + position.y, i_map); // Baixo


    if (1 != frightened_mode) { // Modo normal (n�o assustado)
        unsigned char optimal_direction = 4; // Dire��o �tima para alcan�ar o alvo
        move = 1; // O fantasma pode se mover

        // Encontra a dire��o �tima, considerando que o fantasma n�o pode voltar
        for (unsigned char a = 0; a < 4; a++) {
            if (a == (2 + direction) % 4) { // Ignora a dire��o oposta � atual
                continue;
            }
            else if (0 == walls[a]) { // Se n�o h� parede na dire��o 'a'
                if (4 == optimal_direction) {
                    optimal_direction = a;
                }
                available_ways++;

                // Escolhe a dire��o que minimiza a dist�ncia ao alvo
                if (get_target_distance(a) < get_target_distance(optimal_direction)) {
                    optimal_direction = a;
                }
            }
        }

        // Define a dire��o do fantasma
        if (1 < available_ways) { // Se houver mais de um caminho dispon�vel, escolhe a dire��o �tima
            direction = optimal_direction;
        }
        else { // Caso contr�rio, continua na mesma dire��o ou volta se necess�rio
            if (4 == optimal_direction) {
                direction = (2 + direction) % 4;
            }
            else {
                direction = optimal_direction;
            }
        }
    }
    else { // Modo assustado
        unsigned char random_direction = rand() % 4; // Escolhe uma dire��o aleat�ria

        if (0 == frightened_speed_timer) { // Verifica o timer de velocidade do modo assustado
            move = 1;
            frightened_speed_timer = GHOST_FRIGHTENED_SPEED;


            for (unsigned char a = 0; a < 4; a++) {
                //Eles n�o podem voltar mesmo que estejam assustados.
                if (a == (2 + direction) % 4) {
                    continue;
                }
                else if (0 == walls[a]) {
                    available_ways++;
                }
            }
            //Enquanto houver paredes bloqueando o caminho, ele continuar� escolhendo uma dire��o aleat�ria.
            if (0 < available_ways)
            {
                while (1 == walls[random_direction] || random_direction == (2 + direction) % 4)
                {
                    random_direction = rand() % 4;
                }

                direction = random_direction;
            }
            else
            {
                //Se n�o houver outro caminho, ele volta.
                direction = (2 + direction) % 4;
            }

        }
        else {
            frightened_speed_timer--;
        }
    }

    // Move o fantasma na dire��o escolhida
    if (1 == move) {
        switch (direction) {
        case 0: { // Direita
            position.x += speed;
            break;
        }
        case 1: { // Cima
            position.y -= speed;
            break;
        }
        case 2: { // Esquerda
            position.x -= speed;
            break;
        }
        case 3: { // Baixo
            position.y += speed;
        }
        }

        // Implementa o efeito de warp (atravessar as laterais do mapa)
        if (-CELL_SIZE >= position.x) {
            position.x = CELL_SIZE * MAP_WIDTH - speed;
        }
        else if (position.x >= CELL_SIZE * MAP_WIDTH) {
            position.x = speed - CELL_SIZE;
        }
    }

    // Verifica colis�o com o Pacman
    if (1 == pacman_collision(i_pacman.get_position())) {
        if (0 == frightened_mode) { // Se o fantasma n�o est� assustado, mata o Pacman
            i_pacman.set_dead(1);
            i_pacman.lose_life();
        }
        else { // Se o fantasma est� assustado, foge para casa
            use_door = 1;
            frightened_mode = 2;
            target = home;
        }
    }
}



void Ghost::update_target(unsigned char i_pacman_direction, const Position& i_ghost_0_position, const Position& i_pacman_position)
{
    if (1 == use_door) { // Se o fantasma pode usar a porta
        if (position == target) {
            if (home_exit == target) { // Se chegou na sa�da da casa
                use_door = 0; // N�o pode mais usar a porta
            }
            else if (home == target) { // Se chegou em casa
                frightened_mode = 0; // Deixa de estar assustado
                target = home_exit; // Come�a a sair de casa
            }
        }
    }
    else {
        if (0 == movement_mode) { // Modo Scatter (cada fantasma vai para um canto espec�fico)
            switch (id) {
            case 0: {
                target = { CELL_SIZE * (MAP_WIDTH - 1), 0 };
                break;
            }
            case 1: {
                target = { 0, 0 };
                break;
            }
            case 2: {
                target = { CELL_SIZE * (MAP_WIDTH - 1), CELL_SIZE * (MAP_HEIGHT - 1) };
                break;
            }
            case 3: {
                target = { 0, CELL_SIZE * (MAP_HEIGHT - 1) };
            }
            }
        }
        else { // Modo Chase (cada fantasma persegue o Pacman de uma forma diferente)
            switch (id) {
            case 0: { // Fantasma vermelho persegue diretamente o Pacman
                target = i_pacman_position;
                break;
            }
            case 1: { // Fantasma rosa persegue a 4 c�lulas � frente do Pacman
                target = i_pacman_position;

                switch (i_pacman_direction) {
                case 0: {
                    target.x += CELL_SIZE * GHOST_1_CHASE;
                    break;
                }
                case 1: {
                    target.y -= CELL_SIZE * GHOST_1_CHASE;
                    break;
                }
                case 2: {
                    target.x -= CELL_SIZE * GHOST_1_CHASE;
                    break;
                }
                case 3: {
                    target.y += CELL_SIZE * GHOST_1_CHASE;
                }
                }

                break;
            }
            case 2: { // Fantasma ciano usa uma l�gica mais complexa envolvendo o fantasma vermelho e o Pacman
                target = i_pacman_position;

                switch (i_pacman_direction) {
                case 0: {
                    target.x += CELL_SIZE * GHOST_2_CHASE;
                    break;
                }
                case 1: {
                    target.y -= CELL_SIZE * GHOST_2_CHASE;
                    break;
                }
                case 2: {
                    target.x -= CELL_SIZE * GHOST_2_CHASE;
                    break;
                }
                case 3: {
                    target.y += CELL_SIZE * GHOST_2_CHASE;
                }
                }

                target.x += target.x - i_ghost_0_position.x;
                target.y += target.y - i_ghost_0_position.y;

                break;
            }
            case 3: { // Fantasma laranja persegue o Pacman at� chegar perto, depois vai para o canto
                if (CELL_SIZE * GHOST_3_CHASE <= sqrt(pow(position.x - i_pacman_position.x, 2) + pow(position.y - i_pacman_position.y, 2))) {
                    target = i_pacman_position;
                }
                else {
                    target = { 0, CELL_SIZE * (MAP_HEIGHT - 1) };
                }
            }
            }
        }
    }
}


// Retorna a posi��o do fantasma
Position Ghost::get_position()
{
    return position;
}