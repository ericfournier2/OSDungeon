#include <SFML/Graphics.hpp>
#include "labyrinth.h"
#include "common.h"
#include "databases.h"
#include "brush.h"

#pragma once
class LabyrinthTopView
{
public:
	LabyrinthTopView(const Labyrinth& labyrinth, const Brush& brush, const GroundDb& ground_db, const WallDb& wall_db, const TextureDb& texture_db, const EntityTemplateDb& template_db);
	~LabyrinthTopView();

	CoordF getMapCoord(float screen_x, float screen_y) const;
	void render(sf::RenderTarget& target, float mouse_x, float mouse_y) const;
private:
	sf::Color groundDrawColor(GroundTypeId id) const;
	CoordF getGroundScreenSize() const;
	CoordF getGroundScreenPositionFromMapPosition(int x, int y) const;
	sf::RectangleShape groundRectangle(int x, int y, GroundTypeId id) const;
	void drawWall(sf::RenderTarget& render_target, int x, int y, WallOrientation o, WallTypeId id) const;
	void drawGroundEntity(sf::RenderTarget& render_target, const Entity& entity) const;

	void drawGround(sf::RenderTarget& render_target) const;
	void drawBrush(sf::RenderTarget& render_target, float mouse_x, float mouse_y) const;
	void drawGroundEntities(sf::RenderTarget& render_target) const;
	void drawGrid(sf::RenderTarget& render_target) const;
	void drawWalls(sf::RenderTarget& render_target) const;
	void drawPOV(sf::RenderTarget& render_target) const;

	const Labyrinth& labyrinth;
	const Brush& brush;
	const GroundDb& ground_db;
	const WallDb& wall_db;
	const TextureDb& texture_db;
	const EntityTemplateDb& template_db;
	float grid_spacing = 20.0f;
	float grid_origin_x = 10.0f;
	float grid_origin_y = 10.0f;
	float ground_inset = 1.0;
};