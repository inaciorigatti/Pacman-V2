#pragma once
#include <array>
#include <chrono>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <cstdio> 
#include "Global.h"
#include "DrawText.h"
#include "Pacman.h"
#include "Ghost.h"
#include "GhostManager.h"
#include "ConvertSketch.h"
#include "DrawMap.h"
#include "MapCollision.h"
#include <string>
class Easy
{
private:
    bool game_won;
    unsigned lag;
    unsigned char level;
    sf::Clock gameClock;
    unsigned int game_time;
    std::chrono::time_point<std::chrono::steady_clock> previous_time;
    std::array<std::string, MAP_HEIGHT> map_sketch;
    std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> map;
    std::array<Position, 4> ghost_positions;
    sf::RenderWindow window;
    GhostManager ghost_manager;
    Pacman pacman;
    void reset_positions();
    void saveRanking();
    void resetGame();

public:
    Easy();
    void run();
    std::string format_time(unsigned int total_seconds);
    void displayGameOver();
    void displayVictory();
    std::string dateAtual();

};