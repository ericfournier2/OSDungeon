#include <SFML/Graphics.hpp>
#include <iostream>

#include "labyrinth.h"
#include "labyrinth_edit_view.h"
#include "entity.h"
#include "databases.h"
#include "runner.h"

void labyrinthTestSymmetric();
void labyrinthTestAssymmetric();
Labyrinth getTestLabyrinth();
void testReadWrite();
void testPathFinding();
void testLOS();

int main()
{
    testLOS();

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
    texture_db.loadNewTexture(7, "assets/textures/BunnySprite.png");
    texture_db.loadNewTexture(8, "assets/textures/Bunny2.png", 200, 200);

    EntityTemplateDb template_db;
    template_db.addElement({ 1, MovementType::STATIC, CollisionType::PICKABLE, InteractionType::NONE, 105.0f, 83.0f, -52.5f, -60.0f, 5, {0}, {0}, {0}, {0} });
    template_db.addElement({ 2, MovementType::STATIC, CollisionType::NONE, InteractionType::NONE, 100.0f, 300.0f, -50.0f, -250.0f, 6, {0}, {0}, {0}, {0} });
    template_db.addElement({ 3, MovementType::FOLLOW, CollisionType::NONE, InteractionType::NONE, 50.0f, 50.0f, -25.0f, -40.0f, 7, {0}, {0}, {0}, {0} });
    template_db.addElement({ 4, MovementType::STATIC, CollisionType::NONE, InteractionType::NONE, 200.0f, 200.0f, -100.0f, -180.0f, 8, {0, 1, 0, 2}, {3, 4, 3, 5}, {6, 7, 8}, {6, 7, 8, 7} });

    Databases db(ground_db, wall_db, texture_db, template_db);

    Labyrinth test = Labyrinth(30, 30);
    ShallowEntity chest1 = ShallowEntity( 1, 1, 5, 5, CardinalDirection::NORTH);
    ShallowEntity chest2 = ShallowEntity( 2, 1, 7, 5, CardinalDirection::NORTH);
    ShallowEntity tree1 = ShallowEntity( 3, 2, 5, 7, CardinalDirection::NORTH);
    ShallowEntity tree2 = ShallowEntity( 4, 2, 7, 7, CardinalDirection::NORTH);
    ShallowEntity bunny1 = ShallowEntity(5, 3, 0, 1, CardinalDirection::NORTH);
    ShallowEntity bunny2 = ShallowEntity(6, 3, 1, 0, CardinalDirection::NORTH);
    ShallowEntity bunny3 = ShallowEntity(7, 4, 1, 1, CardinalDirection::NORTH);

    test.getEntityManager().addEntity(chest1);
    test.getEntityManager().addEntity(chest2);
    test.getEntityManager().addEntity(tree1);
    test.getEntityManager().addEntity(tree2);
    test.getEntityManager().addEntity(bunny1);
    test.getEntityManager().addEntity(bunny2);
    test.getEntityManager().addEntity(bunny3);

    //test.loadFromFile("current.labyrinth");
    //Runner runner = Runner(test, db);
    LabyrinthEditView lve = LabyrinthEditView(test, db);
    bool closed = false;
    while (!closed)
    {
        //std::cout << test.printToString();
        closed = lve.processEvents();
        //closed = runner.processEvents() || closed;
        //runner.render();
        lve.render();
        //std::cout << "Done with frame.";
    }
}