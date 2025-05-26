#include "perspective_editor.h"
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

PerspectiveEditor::PerspectiveEditor(OnePointPerspective& perspective_)
	: perspective(perspective_), db({ gdb, wdb, tdb, edb, sdb })
{
	camera_distance = perspective.getCameraDistance();
	aspect_ratio = perspective.getAspectRatio();
	vanish_point = perspective.getVanishingPoint();
	max_depth = perspective.getMaxDepth();
	x_tiles_per_screen = perspective.getScaleX();
	y_tiles_per_screen = perspective.getScaleY();
	perspective.setBaseSize(base_size);

	db.gdb.addElement(GroundInfo({ 1, "Test ground", sf::Color::Green, sf::Color::Blue, 0}));
	db.wdb.addElement(WallInfo({ 1, "Test wall", sf::Color::White, 0}));

	perspectives.push_back(std::make_shared<StaticView>(Labyrinth::buildTriangleLabyrinth(1), db, perspective.getSizeX(), perspective.getSizeY()));
	perspectives.push_back(std::make_shared<StaticView>(Labyrinth::buildTriangleLabyrinth(2), db, perspective.getSizeX(), perspective.getSizeY()));
	perspectives.push_back(std::make_shared<StaticView>(Labyrinth::buildTriangleLabyrinth(3), db, perspective.getSizeX(), perspective.getSizeY()));
	perspectives.push_back(std::make_shared<StaticView>(Labyrinth::buildTriangleLabyrinth(4), db, perspective.getSizeX(), perspective.getSizeY()));
	perspectives.push_back(std::make_shared<StaticView>(Labyrinth::buildTriangleLabyrinth(5), db, perspective.getSizeX(), perspective.getSizeY()));
	perspectives.push_back(std::make_shared<StaticView>(Labyrinth::buildSquareLabyrinth(10), db, perspective.getSizeX(), perspective.getSizeY()));

	refreshViews();
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
	refresh = ImGui::SliderFloat("x tiles per screen", &x_tiles_per_screen, 0.1f, 2.0f, "x tiles per screen = %.2f") || refresh;
	refresh = ImGui::SliderFloat("y tiles per screen", &y_tiles_per_screen, 0.1f, 2.0f, "y tiles per screen = %.2f") || refresh;
	refresh = ImGui::SliderInt("max depth", &max_depth, 0, 10, "max_depth = %d") || refresh;

	return refresh;
}

void PerspectiveEditor::refreshViews(sf::Color clear_color) {
	perspective.setCameraDistance(camera_distance);
	perspective.setVanishingPoint(vanish_point);
	perspective.setMaxDepth(max_depth);
	perspective.setAspectRatio(aspect_ratio);
	perspective.setScaleX(x_tiles_per_screen);
	perspective.setScaleY(y_tiles_per_screen);


	for (int c = 0; c < perspectives.size(); ++c) {
		perspectives[c]->view.setPerspective(perspective);
		perspectives[c]->texture.resize({ static_cast<unsigned int>(perspective.getSizeX()), static_cast<unsigned int>(perspective.getSizeY())});

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
			ImGui::Image(perspectives[c]->texture, { (float)perspective.getSizeX(), (float)perspective.getSizeY() });
			if (c % 3 < 2) {
				ImGui::SameLine();
			}
		}
	}
	ImGui::End();
}