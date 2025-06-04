#include <SFML/System/Clock.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "labyrinth_editor.h"


LabyrinthEditor::LabyrinthEditor(Labyrinth& labyrinth_init, LabyrinthBackground& background_init, Databases& db_init, OnePointPerspective& perspective_init)
	: labyrinth(labyrinth_init), background(background_init),
	  window(sf::VideoMode({ 1600, 900 }), "Edit maze"),
	  perspective(perspective_init),
	  db(db_init), 
	  brush(),
	  brush_editor(brush, db),
	  top_view(labyrinth, labyrinth.getEntityManager(), db, &brush),
	  db_editor(db),
	  perspective_editor(perspective),
	  entity_editor(db),
	  sprite_editor(db)
{
	if (!ImGui::SFML::Init(window)) {
		throw;
	}
	window.setPosition({ 0,0 });
}

LabyrinthEditor::~LabyrinthEditor()
{
	ImGui::SFML::Shutdown();
}

void LabyrinthEditor::drawWallBrushInfo() {
	brush_editor.render();
}

void LabyrinthEditor::runProject() {
	if (!runner) {
		OnePointPerspective pers(perspective);
		pers.setBaseSize(1200);
		runner = std::make_shared<Runner>(labyrinth, background, db, pers);
	}
}

void LabyrinthEditor::renderMenu() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Create")) {
			}
			if (ImGui::MenuItem("Load", "Ctrl+L")) {
				labyrinth.loadFromFile("assets/saves/current.labyrinth");
				db.gdb.readFromFile("assets/saves/ground.db");
				db.wdb.readFromFile("assets/saves/wall.db");
				db.tdb.readFromFile("assets/saves/texture.db");
				db.edb.readFromFile("assets/saves/entities.db");
				db.sdb.readFromFile("assets/saves/sprites.db");
			}
			if (ImGui::MenuItem("Save", "Ctrl+S")) {
				labyrinth.writeToFile("assets/saves/current.labyrinth");
				db.gdb.writeToFile("assets/saves/ground.db");
				db.wdb.writeToFile("assets/saves/wall.db");
				db.tdb.writeToFile("assets/saves/texture.db");
				db.edb.writeToFile("assets/saves/entities.db");
				db.sdb.writeToFile("assets/saves/sprites.db");
			}
			if (ImGui::MenuItem("Save as..")) {
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows")) {
			ImGui::MenuItem("Sprite Editor", NULL, &show_sprite_editor);
			ImGui::MenuItem("Entity Template editor", NULL, &show_entity_editor);
			ImGui::MenuItem("Databases editor", NULL, &show_db_editor);
			ImGui::MenuItem("Perspective editor", NULL, &show_perspective_editor);
			ImGui::MenuItem("Imgui demo", NULL, &show_imgui_demo);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools")) {
			if (ImGui::MenuItem("Run project")) {
				runProject();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void LabyrinthEditor::render() {
	ImGui::SFML::Update(window, deltaClock.restart());
	window.clear();
	renderMenu();
	if (show_imgui_demo) {
		ImGui::ShowDemoWindow();
	}

	top_view.render(window, mouse_x, mouse_y);
	drawWallBrushInfo();
	if (show_db_editor) {
		db_editor.render();
	}
	if (show_perspective_editor) {
		perspective_editor.render();
	}
	if (show_entity_editor) {
		entity_editor.render();
	}
	if (show_sprite_editor) {
		sprite_editor.render();
	}
	if (runner) {
		runner->render();
	}
	ImGui::SFML::Render(window);
	window.display();
}

void LabyrinthEditor::handleKeyPress(const sf::Event::KeyPressed* keyPressed) {
	if (keyPressed->scancode == sf::Keyboard::Scancode::R) {
		runProject();
	}
}

void LabyrinthEditor::handleMouseLeftDown(const sf::Event::MouseButtonPressed* mouseButtonPressed) {
	
}

void LabyrinthEditor::applyBrush() {
	CoordF map_coord = top_view.getMapCoord(mouse_x, mouse_y);
	brush.apply(labyrinth, map_coord.x, map_coord.y);
}

void LabyrinthEditor::handleMouseRightDown(const sf::Event::MouseButtonPressed* mouseButtonPressed) {
	painting_ground = true;
	applyBrush();
}

void LabyrinthEditor::handleMouseMiddleDown(const sf::Event::MouseButtonPressed* mouseButtonPressed) {
	panning = true;
}

void LabyrinthEditor::handleMouseLeftUp(const sf::Event::MouseButtonReleased* mouseButtonReleased) {

}

void LabyrinthEditor::handleMouseRightUp(const sf::Event::MouseButtonReleased* mouseButtonReleased) {
	painting_ground = false;
}

void LabyrinthEditor::handleMouseMiddleUp(const sf::Event::MouseButtonReleased* mouseButtonReleased) {
	panning = false;
}

bool LabyrinthEditor::processEvents()
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
			} else if (mouseButtonPressed->button == sf::Mouse::Button::Middle) {
				handleMouseMiddleDown(mouseButtonPressed);
			}
		}
		else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
			if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
				handleMouseLeftUp(mouseButtonReleased);
			} else if (mouseButtonReleased->button == sf::Mouse::Button::Right) {
				handleMouseRightUp(mouseButtonReleased);
			} else if (mouseButtonReleased->button == sf::Mouse::Button::Middle) {
				handleMouseMiddleUp(mouseButtonReleased);
			}
		}
		else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
			auto old_mouse_x = mouse_x;
			auto old_mouse_y = mouse_y;
			mouse_x = static_cast<float>(mouseMoved->position.x);
			mouse_y = static_cast<float>(mouseMoved->position.y);

			if (panning) {
				top_view.incrementOrigin({ static_cast<float>(mouse_x - old_mouse_x), static_cast<float> (mouse_y - old_mouse_y) });
			}

			if (painting_ground) {
				applyBrush();
			}

		}
		else if (const auto* mouseWheel = event->getIf<sf::Event::MouseWheelScrolled>()) {
			top_view.incrementSpacing((mouseWheel->delta)*3);
		}
	}

	if (runner) {
		if (runner->processEvents()) {
			runner = nullptr;
		}
	}

	return false;
}
