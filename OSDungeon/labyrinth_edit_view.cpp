#include <SFML/System/Clock.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "labyrinth_edit_view.h"


LabyrinthEditView::LabyrinthEditView(Labyrinth& labyrinth_init, GroundDb& ground_db_init, WallDb& wall_db_init, TextureDb& texture_db_init, EntityTemplateDb& template_db_init)
	: labyrinth(labyrinth_init), 
	  window(sf::VideoMode({ 1600, 900 }), "Edit maze"),
	  ground_db(ground_db_init), 
	  wall_db(wall_db_init), 
	  texture_db(texture_db_init),
	  template_db(template_db_init),
	  brush(template_db_init),
	  brush_editor(brush, ground_db_init, wall_db_init, texture_db_init, template_db_init),
	  top_view(labyrinth, brush, ground_db_init, wall_db_init, texture_db_init, template_db_init),
	  db_editor(ground_db_init, wall_db_init, texture_db_init, template_db_init)
{
	ImGui::SFML::Init(window);
	window.setPosition({ 0,0 });
}

LabyrinthEditView::~LabyrinthEditView()
{
	ImGui::SFML::Shutdown();
}

void LabyrinthEditView::drawWallBrushInfo() {
	brush_editor.render();
}

void LabyrinthEditView::render() {
	ImGui::SFML::Update(window, deltaClock.restart());

	ImGui::ShowDemoWindow();

	window.clear();
	top_view.render(window, mouse_x, mouse_y);
	drawWallBrushInfo();
	db_editor.render();
	ImGui::SFML::Render(window);
	window.display();
}

void LabyrinthEditView::handleKeyPress(const sf::Event::KeyPressed* keyPressed) {
	if (keyPressed->scancode == sf::Keyboard::Scancode::Left) {
		labyrinth.turnPovRel(LEFT);
	}
	else if (keyPressed->scancode == sf::Keyboard::Scancode::Up) {
		labyrinth.advance();
	}
	else if (keyPressed->scancode == sf::Keyboard::Scancode::Right) {
		labyrinth.turnPovRel(RIGHT);
	}
	else if (keyPressed->scancode == sf::Keyboard::Scancode::Down) {
		labyrinth.moveBack();
	}
	else if (keyPressed->scancode == sf::Keyboard::Scancode::S) {
		labyrinth.writeToFile("assets/saves/current.labyrinth");
		ground_db.writeToFile("assets/saves/ground.db");
		wall_db.writeToFile("assets/saves/wall.db");
		texture_db.writeToFile("assets/saves/texture.db");

	}
	else if (keyPressed->scancode == sf::Keyboard::Scancode::L) {
		labyrinth.loadFromFile("assets/saves/current.labyrinth");
		ground_db.readFromFile("assets/saves/ground.db");
		wall_db.readFromFile("assets/saves/wall.db");
		texture_db.readFromFile("assets/saves/texture.db");

	}
}

void LabyrinthEditView::handleMouseLeftDown(const sf::Event::MouseButtonPressed* mouseButtonPressed) {
	
}

void LabyrinthEditView::applyBrush() {
	CoordF map_coord = top_view.getMapCoord(mouse_x, mouse_y);
	brush.apply(labyrinth, map_coord.x, map_coord.y);
}

void LabyrinthEditView::handleMouseRightDown(const sf::Event::MouseButtonPressed* mouseButtonPressed) {
	painting_ground = true;
	applyBrush();
}

void LabyrinthEditView::handleMouseLeftUp(const sf::Event::MouseButtonReleased* mouseButtonReleased) {

}

void LabyrinthEditView::handleMouseRightUp(const sf::Event::MouseButtonReleased* mouseButtonReleased) {
	painting_ground = false;
}

bool LabyrinthEditView::processEvents()
{
	while (const std::optional event = window.pollEvent()) {
		ImGui::SFML::ProcessEvent(window, *event);
		if (event->is<sf::Event::Closed>()) {
			window.close();
			return true;
		}
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
			handleKeyPress(keyPressed);
		}
		else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
			if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
				handleMouseLeftDown(mouseButtonPressed);
			} else if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
				handleMouseRightDown(mouseButtonPressed);
			}
		}
		else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
			if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
				handleMouseLeftUp(mouseButtonReleased);
			} else if (mouseButtonReleased->button == sf::Mouse::Button::Right) {
				handleMouseRightUp(mouseButtonReleased);
			}
		}
		else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
			mouse_x = static_cast<float>(mouseMoved->position.x);
			mouse_y = static_cast<float>(mouseMoved->position.y);
			if (painting_ground) {
				applyBrush();
			}
		}
	}

	return false;
}
