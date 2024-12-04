#pragma once // Evita múltiplas inclusões deste arquivo no mesmo projeto.

// Constantes relacionadas ao tamanho e escala de elementos no jogo.
constexpr unsigned char CELL_SIZE = 16; // Tamanho de uma célula no mapa.
constexpr unsigned char FONT_HEIGHT = 16; // Altura da fonte usada para exibição de texto.

// Constantes relacionadas ao comportamento dos fantasmas.
constexpr unsigned char GHOST_1_CHASE = 2; // Número de células à frente do Pacman que o fantasma rosa visa.
constexpr unsigned char GHOST_2_CHASE = 1; // Número de células à frente do Pacman usado pelo fantasma azul para calcular o alvo.
constexpr unsigned char GHOST_3_CHASE = 4; // Distância mínima entre o fantasma laranja e o Pacman para ativar o modo "scatter".
constexpr unsigned char GHOST_ANIMATION_FRAMES = 6; // Número de quadros na animação do corpo dos fantasmas.
constexpr unsigned char GHOST_ANIMATION_SPEED = 4; // Velocidade da animação dos fantasmas.
constexpr unsigned char GHOST_ESCAPE_SPEED = 4; // Velocidade do fantasma ao fugir após o Pacman estar energizado.
constexpr unsigned char GHOST_FRIGHTENED_SPEED = 3; // Velocidade do fantasma no modo assustado. Valores maiores resultam em movimentos mais lentos.
constexpr unsigned char GHOST_SPEED = 1; // Velocidade normal dos fantasmas.

// Dimensões do mapa do jogo.
constexpr unsigned char MAP_HEIGHT = 21; // Altura do mapa em células.
constexpr unsigned char MAP_WIDTH = 21; // Largura do mapa em células.

// Constantes relacionadas ao Pacman.
constexpr unsigned char PACMAN_ANIMATION_FRAMES = 6; // Número de quadros na animação do Pacman.
constexpr unsigned char PACMAN_ANIMATION_SPEED = 4; // Velocidade da animação do Pacman.
constexpr unsigned char PACMAN_DEATH_FRAMES = 12; // Número de quadros na animação de morte do Pacman.
constexpr unsigned char PACMAN_SPEED = 2; // Velocidade de movimento do Pacman.

// Constante para redimensionar a tela do jogo.
constexpr unsigned char SCREEN_RESIZE = 2; // Fator de escala para redimensionar a resolução do jogo.

// Constantes relacionadas ao tempo e duração de estados no jogo.
constexpr unsigned short CHASE_DURATION = 1024; // Duração (em quadros) do modo de perseguição dos fantasmas.
constexpr unsigned short ENERGIZER_DURATION = 512; // Duração (em quadros) do efeito de energizador.
constexpr unsigned short FRAME_DURATION = 16667; // Duração de um quadro (em microssegundos).
constexpr unsigned short GHOST_FLASH_START = 64; // Tempo (em quadros) antes do início do efeito de "piscar" do fantasma.
constexpr unsigned short LONG_SCATTER_DURATION = 512; // Duração longa do modo "scatter".
constexpr unsigned short SHORT_SCATTER_DURATION = 256; // Duração curta do modo "scatter".
constexpr unsigned short CHERRY_DURATION = 5; //Duração pontos duplos
extern bool dark_mode; //Modo escuro - desafio

// Enumeração para representar diferentes tipos de células no mapa do jogo.
enum Cell {
	Door,      // Porta no mapa.
	Empty,     // Célula vazia.
	Energizer, // Célula com energizador.
	Pellet,    // Célula com pellet (comida comum).
	Wall,      // Célula contendo uma parede.
	Cherry     // Cereja dobro pontos
};

// Enumeração para representar os diferentes estados do jogo.
enum class GameState {
	Menu,      // Estado do menu inicial.
	Playing,   // Estado durante o jogo.
	GameOver   // Estado quando o jogo termina.
};

// Variável global que representa o estado atual do jogo.
extern GameState currentState;

// Estrutura para representar a posição (x, y) de elementos no jogo.
struct Position {
	short x; // Coordenada x.
	short y; // Coordenada y.

	// Sobrecarga do operador "==" para comparar posições.
	bool operator==(const Position& i_position) {
		return this->x == i_position.x && this->y == i_position.y;
	}
};
