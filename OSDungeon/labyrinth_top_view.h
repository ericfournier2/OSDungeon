#include <SFML/Graphics.hpp>
#include "labyrinth.h"
#include "common.h"
#include "databases.h"
#include "brush.h"
#include "entity.h"

#pragma once
class LabyrinthTopView
{
public:
	LabyrinthTopView(const Labyrinth& labyrinth, const EntityManager& entities,  const Databases& db, const Brush* brush = 0 );
	~LabyrinthTopView();

	CoordF getMapCoord(float screen_x, float screen_y) const;
	void render(sf::RenderTarget& target, float mouse_x, float mouse_y) const;
	float getSpacing() const { return grid_spacing; }
	void setSpacing(float spacing) { grid_spacing = spacing; }
	void incrementSpacing(float increment) { grid_spacing += increment; }
	CoordF getOrigin() const { return { grid_origin_x, grid_origin_y }; }
	void setOrigin(CoordF origin) { grid_origin_x = origin.x; grid_origin_y = origin.y; }
	void incrementOrigin(CoordF increment) { grid_origin_x += increment.x; grid_origin_y += increment.y; }
private:
	sf::Color groundDrawColor(GroundId id) const;
	CoordF getGroundScreenSize() const;
	CoordF getGroundScreenPositionFromMapPosition(int x, int y, float sub_x = 0.0f, float sub_y = 0.0f) const;
	sf::RectangleShape groundRectangle(int x, int y, GroundId id) const;
	void drawWall(sf::RenderTarget& render_target, int x, int y, WallOrientation o, WallId id) const;
	void drawGroundEntity(sf::RenderTarget& render_target, const Entity& entity) const;

	void drawGround(sf::RenderTarget& render_target) const;
	void drawBrush(sf::RenderTarget& render_target, float mouse_x, float mouse_y) const;
	void drawGroundEntities(sf::RenderTarget& render_target) const;
	void drawGrid(sf::RenderTarget& render_target) const;
	void drawWalls(sf::RenderTarget& render_target) const;
	void drawPOV(sf::RenderTarget& render_target) const;

	const Labyrinth& labyrinth;
	const EntityManager& entities;
	const Brush* brush;
	const Databases& db;
	float grid_spacing = 20.0f;
	float grid_origin_x = 20.0f;
	float grid_origin_y = 30.0f;
	float ground_inset = 1.0;
};