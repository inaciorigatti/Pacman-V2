#pragma once

class Ghost
{
	bool movement_mode;
	bool use_door;

	//Current direction.
	unsigned char direction;
	//0 - nao esta estressado
	//1 - estressado
	//2 - indo pra casa
	unsigned char frightened_mode;

	unsigned char frightened_speed_timer;
	//0 - Red
	//1 - Pink
	//2 - Blue
	//3 - Orange
	unsigned char id;

	unsigned short animation_timer;

	//Os fantasmas vao voltar 
	Position home;
	//Ghost sair da sua 'casa'
	Position home_exit;
	//Posicao atual 
	Position position;
	//Alvo atual

	Position target;
public:
	Ghost(unsigned char i_id);

	bool pacman_collision(const Position& i_pacman_position);

	float get_target_distance(unsigned char i_direction);

	void draw(bool i_flash, sf::RenderWindow& i_window);
	void reset(const Position& i_home, const Position& i_home_exit);
	void set_position(short i_x, short i_y);
	void switch_mode();
	void update(unsigned char i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, Ghost& i_ghost_0, Pacman& i_pacman);
	void update_target(unsigned char i_pacman_direction, const Position& i_ghost_0_position, const Position& i_pacman_position);

	Position get_position();
};