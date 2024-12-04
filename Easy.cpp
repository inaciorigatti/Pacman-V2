#include "Easy.h"
#include "Menu.h"
#include "Textbox.h"
#include "Ranking.h" 

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>


Easy::Easy() :
    game_won(0),
    lag(0),
    level(0),
    game_time(0),
    map_sketch({
        " ################### ",
        " #........#........# ",
        " #o##.###.#.###.##o# ",
        " #........c........# ",
        " #.##.#.#####.#.##.# ",
        " #....#...#...#....# ",
        " ####.### # ###.#### ",
        "    #.#   0   #.#    ",
        "#####.# ##=## #.#####",
        "     .  #123#  .     ",
        "#####.# ##### #.#####",
        "    #.#       #.#    ",
        " ####.# ##### #.#### ",
        " #........#........# ",
        " #.##.###.#.###.##.# ",
        " #o.#.....P.....#.o# ",
        " ##.#.#.#####.#.#.## ",
        " #....#...#...#....# ",
        " #.######.#.######.# ",
        " #........c........# ",
        " ################### "
        }),
    window(sf::VideoMode(CELL_SIZE* MAP_WIDTH* SCREEN_RESIZE, (FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT)* SCREEN_RESIZE), "Pac-Man", sf::Style::Close)
{
    window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * MAP_WIDTH, FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT)));
    srand(static_cast<unsigned>(time(0)));
    map = convert_sketch(map_sketch, ghost_positions, pacman);
    ghost_manager.reset(level, ghost_positions);
    previous_time = std::chrono::steady_clock::now();
    gameClock.restart();
}

// Função que retorna a data atual no formato DD/MM/AAAA
std::string actualDate() {
    time_t now = time(0);
    tm localTime;
    localtime_s(&localTime, &now);

    std::stringstream data;
    data << localTime.tm_mday << "/"
        << localTime.tm_mon + 1 << "/"
        << localTime.tm_year + 1900;

    return data.str();
}

// Função para formatar o tempo total em formato HH:MM:SS
std::string Easy::format_time(unsigned int total_seconds) {
    unsigned int hours = total_seconds / 3600;
    unsigned int minutes = (total_seconds % 3600) / 60;
    unsigned int seconds = total_seconds % 60;
    char buffer[9];
    snprintf(buffer, sizeof(buffer), "%02u:%02u:%02u", hours, minutes, seconds);
    return std::string(buffer);
}

// Exibe a mensagem de game over e salva o ranking
void Easy::displayGameOver() {
    window.clear();
    draw_text(1, 0, 0, "Voce perdeu! Mais sorte na proxima!", window);
    window.display();
    sf::sleep(sf::seconds(3));

    // Salvar o ranking
    saveRanking();

    // Reset do jogo
    game_won = 0;
    level = 0;
    map = convert_sketch(map_sketch, ghost_positions, pacman);
    ghost_manager.reset(level, ghost_positions);
    pacman.reset();

    // Retornando ao menu
    window.close();
    Menu menu(window);
    menu.run();
}

// Exibe a mensagem de vitória e salva o ranking
void Easy::displayVictory() {
    window.clear();
    draw_text(1, 0, 0, "Voce ganhou! Parabens!", window);
    window.display();
    sf::sleep(sf::seconds(3));

    // Salvar o ranking
    saveRanking();

    // Reset do jogo
    game_won = 0;
    level = 0;
    map = convert_sketch(map_sketch, ghost_positions, pacman);
    ghost_manager.reset(level, ghost_positions);
    pacman.reset();

    // Retornando ao menu
    window.close();
    Menu menu(window);
    menu.run();
}

void Easy::saveRanking() {
    Ranking ranking;
    sf::Font arial;
    arial.loadFromFile("C:/Windows/Fonts/arial.ttf");

    // Inicializa a caixa de texto para entrada do nome do jogador
    Textbox textbox(20, sf::Color::White, true);
    textbox.setFont(arial);
    textbox.setPosition({ 100, 255 });

    std::string playerName;
    sf::Event event;

    // Loop para captura do nome
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::TextEntered) {
                textbox.typedOn(event);
                playerName = textbox.getText();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                if (!playerName.empty()) {
                    // Adiciona o registro ao ranking e salva
                    ranking.addRecord(playerName, pacman.get_score(), game_time, actualDate());
                    ranking.saveToFile();

                    // Fecha a janela e retorna ao menu
                    window.close();
                    Menu menu(window);
                    menu.run();
                    return;
                }
            }
        }

        // Renderiza a interface de entrada do nome
        window.clear();
        draw_text(1, 0, 0, "Digite seu nome:", window);
        textbox.drawTo(window);
        window.display();
    }
}



// Função principal de execução do jogo
void Easy::run() {
    sf::Event event;
    while (window.isOpen()) {
        unsigned delta_time = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now() - previous_time).count();
        lag += delta_time;
        previous_time += std::chrono::microseconds(delta_time);

        while (FRAME_DURATION <= lag) {
            lag -= FRAME_DURATION;

            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            sf::Time elapsedTime = gameClock.getElapsedTime();
            if (elapsedTime.asSeconds() >= 1.0f) {
                game_time++;
                gameClock.restart();
            }

            if (0 == game_won && 0 == pacman.get_dead()) {
                // Verifica condição de vitória
                game_won = 1;
                for (const auto& column : map) {
                    for (const auto& cell : column) {
                        if (Cell::Pellet == cell) {
                            game_won = 0;
                            break;
                        }
                    }
                    if (!game_won) {
                        break;
                    }
                }
                if (game_won) {
                    displayVictory();
                }
                else {
                    pacman.update(level, map);
                    ghost_manager.update(level, map, pacman);
                    if (pacman.get_dead()) {
                        if (pacman.get_lives() == 0) {
                            displayGameOver();
                        }
                        else {
                            pacman.set_position(160, 240);
                            pacman.update(level, map);
                            ghost_manager.reset(level, ghost_positions);
                        }
                    }
                }
            }

            window.clear();
            if (0 == game_won && 0 == pacman.get_dead()) {
                draw_map(map, window);
                ghost_manager.draw(GHOST_FLASH_START >= pacman.get_energizer_timer(), window);
                draw_text(0, 0, CELL_SIZE * MAP_WIDTH, "Pontos:" + std::to_string(pacman.get_score()), window);
                draw_text(0, 85 + FONT_HEIGHT, CELL_SIZE * MAP_WIDTH, "Vidas:" + std::to_string(pacman.get_lives()), window);
                draw_text(0, 165, CELL_SIZE * MAP_WIDTH, "Mov:" + std::to_string(pacman.get_move_counter()), window);
                draw_text(0, 220, CELL_SIZE * MAP_WIDTH, "Tempo:" + format_time(game_time), window);
            }
            // Modo escuro - desafio // precisa corrigir a aparição do mapa e fantasma de acordo com o tamanho do retangulo de luz criado.
            if (dark_mode) {

                if (game_time <= 50 && 1 == game_won) { /*se o jogo foi vencido antes do tempo */
                    displayVictory();
                }
                else if (game_time > 50 && 0 == game_won) { /*se o jogo foi vencido antes do tempo */
                    displayGameOver();
                }

                // Define o tamanho do retângulo de luz
                sf::Vector2f rectSize(10 * CELL_SIZE, 10 * CELL_SIZE); // Ajuste conforme necessário

                // Cria o retângulo
                sf::RectangleShape rect(rectSize);

                // Definir a cor de preenchimento como transparente
                rect.setFillColor(sf::Color::Transparent);

                rect.setOutlineColor(sf::Color::Black);

                rect.setOutlineThickness(500);

                // Define a posição do retângulo centralizada no Pac-Man
                rect.setPosition(pacman.get_position().x - rectSize.x / 2.f, pacman.get_position().y - rectSize.y / 2.f);

                // Desenha o retângulo na janela
                window.draw(rect);

                draw_text(0, 0, CELL_SIZE * MAP_WIDTH, "Pontos:" + std::to_string(pacman.get_score()), window);
                draw_text(0, 85 + FONT_HEIGHT, CELL_SIZE * MAP_WIDTH, "Vidas:" + std::to_string(pacman.get_lives()), window);
                draw_text(0, 165, CELL_SIZE * MAP_WIDTH, "Mov:" + std::to_string(pacman.get_move_counter()), window);
                draw_text(0, 220, CELL_SIZE * MAP_WIDTH, "Tempo:" + format_time(game_time), window);
            }
            pacman.draw(game_won, window);
            window.display();
        }
    }
}