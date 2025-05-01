#include <SFML/System/Clock.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "labyrinth_edit_view.h"


LabyrinthEditView::LabyrinthEditView(Labyrinth& labyrinth_init, Databases& db_init)
	: labyrinth(labyrinth_init), 
	  window(sf::VideoMode({ 1600, 900 }), "Edit maze"),
	  db(db_init), 
	  brush(db.edb),
	  brush_editor(brush, db),
	  top_view(labyrinth, labyrinth.getEntityManager(), db, &brush),
	  db_editor(db)
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
	perspective_editor.render();
	if (runner) {
		runner->render();
	}
	ImGui::SFML::Render(window);
	window.display();
}

void LabyrinthEditView::handleKeyPress(const sf::Event::KeyPressed* keyPressed) {
	/*if (keyPressed->scancode == sf::Keyboard::Scancode::Left) {
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
	else*/ if (keyPressed->scancode == sf::Keyboard::Scancode::S) {
		labyrinth.writeToFile("assets/saves/current.labyrinth");
		db.gdb.writeToFile("assets/saves/ground.db");
		db.wdb.writeToFile("assets/saves/wall.db");
		db.tdb.writeToFile("assets/saves/texture.db");
		db.edb.writeToFile("assets/saves/entities.db");

	}
	else if (keyPressed->scancode == sf::Keyboard::Scancode::L) {
		labyrinth.loadFromFile("assets/saves/current.labyrinth");
		db.gdb.readFromFile("assets/saves/ground.db");
		db.wdb.readFromFile("assets/saves/wall.db");
		db.tdb.readFromFile("assets/saves/texture.db");
		db.edb.readFromFile("assets/saves/entities.db");
	} else if (keyPressed->scancode == sf::Keyboard::Scancode::R) {
		if (!runner) {
			runner = std::make_shared<Runner>(labyrinth, db);
		}
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

	if (runner) {
		if (runner->processEvents()) {
			runner = nullptr;
		}
	}

	return false;
}
