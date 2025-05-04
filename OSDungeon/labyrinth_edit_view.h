#include <memory>
#include <SFML/Graphics.hpp>
#include "labyrinth.h"
#include "common.h"
#include "databases.h"
#include "brush.h"
#include "brush_editor.h"
#include "labyrinth_top_view.h"
#include "db_editor.h"
#include "runner.h"
#include "perspective_editor.h"
#include "entity_editor.h"

#pragma once
class LabyrinthEditView
{
public:
	LabyrinthEditView(Labyrinth& labyrinth, Databases& db);
	~LabyrinthEditView();

	void render();
	
	bool processEvents();
private:
	void handleKeyPress(const sf::Event::KeyPressed* keyPressed);
	void handleMouseLeftDown(const sf::Event::MouseButtonPressed* mouseButtonPressed);
	void applyBrush();
	void handleMouseRightDown(const sf::Event::MouseButtonPressed* mouseButtonPressed);
	void handleMouseLeftUp(const sf::Event::MouseButtonReleased* mouseButtonReleased);
	void handleMouseRightUp(const sf::Event::MouseButtonReleased* mouseButtonReleased);
	void drawWallBrushInfo();

	Labyrinth& labyrinth;
	sf::RenderWindow window;
	float mouse_x = 0.0f;
	float mouse_y = 0.0f;
	sf::Clock deltaClock;
	Databases& db;
	bool painting_ground = false;
	Brush brush;
	BrushEditor brush_editor;
	LabyrinthTopView top_view;
	DatabaseEditor db_editor;
	std::shared_ptr<Runner> runner;
	PerspectiveEditor perspective_editor;
	EntityEditor entity_editor;
};

