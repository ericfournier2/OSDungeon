#pragma once
#include "labyrinth.h"
#include "labyrinth_pov.h"
#include "labyrinth_view.h"
#include <SFML/Audio.hpp>

class Runner {
public:
	Runner(const Labyrinth& labyrinth, const Databases& db);

	void render();
	void handleKeyPress(const sf::Event::KeyPressed* keyPressed);
	bool processEvents();
private:
	void tick();

	Labyrinth labyrinth;
	ShallowEntityManager entities;
	LabyrinthPOV pov;
	const Databases& db;
	sf::RenderWindow window;
	LabyrinthView lv;

	sf::SoundBuffer footsteps_buffer = sf::SoundBuffer("assets/sounds/footstep.wav");
	sf::SoundBuffer thunk_buffer = sf::SoundBuffer("assets/sounds/thunk.wav");
	sf::Sound footstep = sf::Sound(footsteps_buffer);
	sf::Sound thunk = sf::Sound(thunk_buffer);
};