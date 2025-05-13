#pragma once
#include "labyrinth.h"
#include "labyrinth_pov.h"
#include "labyrinth_view.h"
#include "game_state.h"
#include <SFML/Audio.hpp>


class Runner {
public:
	Runner(const Labyrinth& labyrinth, const LabyrinthBackground& background, const Databases& db);

	void render();
	
	bool processEvents();
private:
	void tick();
	void tickBackground();
	void printCenteredDialogText(const std::string& message, int line_number, float text_left_start, float text_top_start, float text_max_width);
	void displayMessageBox();
	void handleKeyPress(const sf::Event::KeyPressed* keyPressed);
	void handleKeyPressWaitingForInput(const sf::Event::KeyPressed* keyPressed);
	void handleKeyPressOther(const sf::Event::KeyPressed* keyPressed);

	unsigned int window_width = 1600;
	unsigned int window_height = 900;
	int lv_width = window_width;
	int lv_height = window_height;

	float dialog_left_margin = 0.15f;
	float dialog_top_margin = 0.15f;
	float dialog_right_margin = 0.15f;
	float dialog_bottom_margin = 0.15f;

	float text_left_margin = 0.05f;
	float text_top_margin = 0.05f;
	float text_right_margin = 0.05f;
	float text_bottom_margin = 0.10f;

	int dialog_char_size = 24;

	Labyrinth labyrinth;
	LabyrinthBackground background;
	EntityManager entities;
	LabyrinthPOV pov;
	const Databases& db;
	sf::RenderWindow window;
	LabyrinthView lv;

	sf::SoundBuffer footsteps_buffer = sf::SoundBuffer("assets/sounds/footstep.wav");
	sf::SoundBuffer thunk_buffer = sf::SoundBuffer("assets/sounds/thunk.wav");
	sf::SoundBuffer coin_buffer = sf::SoundBuffer("assets/sounds/coin.mp3");
	sf::Sound footstep = sf::Sound(footsteps_buffer);
	sf::Sound thunk = sf::Sound(thunk_buffer);
	sf::Sound coin = sf::Sound(coin_buffer);

	GameState gs;

	sf::Font font;
	bool should_exit = false;

	sf::Clock animation_clock;
	sf::Time last_time = animation_clock.getElapsedTime();
};