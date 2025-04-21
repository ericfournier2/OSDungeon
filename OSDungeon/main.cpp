#include <SFML/Graphics.hpp>
#include <iostream>

#include "labyrinth.h"
#include "labyrinth_view.h"
#include "labyrinth_edit_view.h"
#include "entity.h"
#include "databases.h"
#include "db_editor.h"

void labyrinthTestSymmetric();
void labyrinthTestAssymmetric();
Labyrinth getTestLabyrinth();
void testReadWrite();
void testPathFinding();

int main()
{
    GroundDb ground_db;
    //ground_db.addElement(GroundInfo({ 0, sf::Color::White, 0 }));
    ground_db.addElement(GroundInfo({ 1, sf::Color::White, sf::Color::Blue, 4 }));
    ground_db.addElement(GroundInfo({ 2, sf::Color::Green, sf::Color::Blue, 4 }));
    ground_db.addElement(GroundInfo({ 3, sf::Color::Red, sf::Color::Blue, 4 }));

    WallDb wall_db;
    //wall_db.addElement(WallInfo({ 0, sf::Color::White, 0 }));
    wall_db.addElement(WallInfo({ 1, sf::Color::White, 3 }));
    wall_db.addElement(WallInfo({ 2, sf::Color::Green, 3 }));
    wall_db.addElement(WallInfo({ 3, sf::Color::Red, 3 }));

    TextureDb texture_db;
    texture_db.loadNewTexture(1, "assets/textures/Elora.png");
    texture_db.loadNewTexture(2, "assets/textures/Coralie.png");
    texture_db.loadNewTexture(3, "assets/textures/BigBricksTexture20.png");
    texture_db.loadNewTexture(4, "assets/textures/Ground2.png");
    texture_db.loadNewTexture(5, "assets/textures/ChestSprite.png");
    texture_db.loadNewTexture(6, "assets/textures/TreeSprite.png");

    EntityTemplateDb template_db;
    template_db.addElement({ 1, DOODAD, 105.0f, 83.0f, -52.5f, -60.0f, 5 });
    template_db.addElement({ 2, DOODAD, 100.0f, 300.0f, -50.0f, -250.0f, 6 });

    Labyrinth test = Labyrinth(20, 20);
    Entity chest1 = Entity({ 1, 1, 5, 5, CardinalDirection::NORTH }, template_db);
    Entity chest2 = Entity({ 2, 1, 7, 5, CardinalDirection::NORTH }, template_db);
    Entity tree1 = Entity({ 3, 2, 5, 7, CardinalDirection::NORTH }, template_db);
    Entity tree2 = Entity({ 4, 2, 7, 7, CardinalDirection::NORTH }, template_db);

    test.addEntity(chest1);
    test.addEntity(chest2);
    test.addEntity(tree1);
    test.addEntity(tree2);

    //test.loadFromFile("current.labyrinth");
    LabyrinthView lv = LabyrinthView(test, ground_db, wall_db, texture_db, 400, 300);
    LabyrinthEditView lve = LabyrinthEditView(test, ground_db, wall_db, texture_db, template_db);
    bool closed = false;
    while (!closed)
    {
        //std::cout << test.printToString();
        closed = lve.processEvents();
        closed = lv.processEvents() || closed;
        lv.render();
        lve.render();
        //std::cout << "Done with frame.";
    }
}