#include <SFML/Graphics.hpp>
#include <iostream>

#include "labyrinth.h"
#include "labyrinth_view.h"
#include "labyrinth_edit_view.h"
#include "entity.h"
#include "databases.h"

void labyrinthTestSymmetric();
void labyrinthTestAssymmetric();
Labyrinth getTestLabyrinth();
void testReadWrite();

int main()
{
    sf::Font font;
    if (!font.openFromFile("LEMONMILK - Regular.otf")) {
        assert("Font failed to load.");
    }
    sf::Texture chest;
    chest.loadFromFile("Sprite.png");

    GroundDb ground_db;
    //ground_db.addElement(GroundInfo({ 0, sf::Color::White, 0 }));
    ground_db.addElement(GroundInfo({ 1, sf::Color::White, 4 }));
    ground_db.addElement(GroundInfo({ 2, sf::Color::Green, 4 }));
    ground_db.addElement(GroundInfo({ 3, sf::Color::Red, 4 }));

    WallDb wall_db;
    //wall_db.addElement(WallInfo({ 0, sf::Color::White, 0 }));
    wall_db.addElement(WallInfo({ 1, sf::Color::White, 3 }));
    wall_db.addElement(WallInfo({ 2, sf::Color::Green, 3 }));
    wall_db.addElement(WallInfo({ 3, sf::Color::Red, 3 }));

    TextureDb texture_db;


    Labyrinth test = Labyrinth(0, 0);
    Entity ent1 = Entity(&test, &chest, 5, 5, CardinalDirection::NORTH, 105.0f, 83.0f, -52.5f, -60.0f);
    test.addEntity(ent1);

    test.loadFromFile("current.labyrinth");
    LabyrinthView lv = LabyrinthView(test, ground_db, wall_db, texture_db, font, 400, 300);
    LabyrinthEditView lve = LabyrinthEditView(test);
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