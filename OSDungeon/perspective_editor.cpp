#include "perspective_editor.h"
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

PerspectiveEditor::PerspectiveEditor()
	: db({ gdb, wdb, tdb, edb })
{
	db.gdb.addElement(GroundInfo({ 1, sf::Color::Green, sf::Color::Blue, 0 }));
	db.wdb.addElement(WallInfo({ 1, sf::Color::White, 0 }));

	perspectives.push_back(std::make_shared<StaticView>(Labyrinth::buildTriangleLabyrinth(1), db, size_x, size_y));
	perspectives.push_back(std::make_shared<StaticView>(Labyrinth::buildTriangleLabyrinth(2), db, size_x, size_y));
	perspectives.push_back(std::make_shared<StaticView>(Labyrinth::buildTriangleLabyrinth(3), db, size_x, size_y));
	perspectives.push_back(std::make_shared<StaticView>(Labyrinth::buildTriangleLabyrinth(4), db, size_x, size_y));
	perspectives.push_back(std::make_shared<StaticView>(Labyrinth::buildTriangleLabyrinth(5), db, size_x, size_y));
	perspectives.push_back(std::make_shared<StaticView>(Labyrinth::buildSquareLabyrinth(10), db, size_x, size_y));
}

void PerspectiveEditor::saveImagesToFile() {
	// Set colors to white.
	GroundInfo g_info = db.gdb.getElement(1);
	GroundInfo g_info_white = g_info;
	g_info_white.ground_color = sf::Color::White;
	g_info_white.ceiling_color = sf::Color::White;
	db.gdb.updateElement(g_info_white);

	WallInfo w_info = db.wdb.getElement(1);
	WallInfo w_info_white = w_info;
	w_info_white.color = sf::Color::White;
	db.wdb.updateElement(w_info_white);

	// Re-render at higher resolution.
	base_size = 1200;
	refreshViews(sf::Color::White);

	for (int c = 0; c < perspectives.size(); ++c) {
		sf::Image i = perspectives[c]->texture.getTexture().copyToImage();
		i.saveToFile(std::string("Perspective ") + std::to_string(c) + std::string(".png"));
	}

	// Restore colors and sizes and re-render.
	db.gdb.updateElement(g_info);
	db.wdb.updateElement(w_info);
	base_size = 400;
	refreshViews();
}

void PerspectiveEditor::renderMenu() {
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Menu")) {
			if (ImGui::MenuItem("Save images", NULL, &menu_selected)) {
				saveImagesToFile();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

bool PerspectiveEditor::renderWidgets() {
	bool refresh = false;

	refresh = ImGui::SliderFloat("camera distance", &camera_distance, 0.0f, 1.0f, "camera distance = %.3f") || refresh;
	refresh = ImGui::SliderFloat("aspect ratio", &aspect_ratio, 0.5f, 2.0f, "aspect ratio = %.2f") || refresh;
	refresh = ImGui::SliderFloat("vanishing point x", &vanish_point.x, 0.0f, 1.0f, "x vanish point = %.2f") || refresh;
	refresh = ImGui::SliderFloat("vanishing point y", &vanish_point.y, 0.0f, 1.0f, "y vanish point = %.2f") || refresh;
	refresh = ImGui::SliderInt("max depth", &max_depth, 0, 10, "max_depth = %d") || refresh;

	return refresh;
}

void PerspectiveEditor::refreshViews(sf::Color clear_color) {
	size_x = (unsigned int)(base_size * aspect_ratio);
	size_y = (unsigned int)(base_size / aspect_ratio);

	for (int c = 0; c < perspectives.size(); ++c) {
		perspectives[c]->view.setCameraDistance(camera_distance);
		perspectives[c]->view.setVanishingPoint(vanish_point);
		perspectives[c]->view.setMaxDepth(max_depth);
		perspectives[c]->view.setSizeX(size_x);
		perspectives[c]->view.setSizeY(size_y);
		perspectives[c]->texture.resize({ size_x, size_y });

		perspectives[c]->texture.clear(clear_color);
		perspectives[c]->view.render();
	}
}

void PerspectiveEditor::render() {
	if (ImGui::Begin("Perspective editor", NULL, ImGuiWindowFlags_MenuBar)) {
		renderMenu();

		if (renderWidgets()) {
			refreshViews();
		}

		for (int c = 0; c < perspectives.size(); ++c) {
			ImGui::Image(perspectives[c]->texture, { (float)size_x, (float)size_y });
			if (c % 3 < 2) {
				ImGui::SameLine();
			}
		}
	}
	ImGui::End();
}