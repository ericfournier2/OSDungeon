#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <thread>

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
    ground_db.addElement(GroundInfo({ 1, sf::Color::White, sf::Color::Blue, 4, false }));
    ground_db.addElement(GroundInfo({ 2, sf::Color::Green, sf::Color::Blue, 4, false }));
    ground_db.addElement(GroundInfo({ 3, sf::Color::Red, sf::Color::Blue, 4, false }));
    ground_db.addElement(GroundInfo({ 4, sf::Color::White, sf::Color::White, 10, false }));

    WallDb wall_db;
    //wall_db.addElement(WallInfo({ 0, sf::Color::White, 0 }));
    wall_db.addElement(WallInfo({ 1, sf::Color::White, 3 }));
    wall_db.addElement(WallInfo({ 2, sf::Color::Green, 3 }));
    wall_db.addElement(WallInfo({ 3, sf::Color::Red, 3 }));

    TextureDb texture_db;
    auto volcan_texture = texture_db.loadNewTexture(1, "assets/textures/Elora.png", "Volcan");
    auto colored_wall_texture = texture_db.loadNewTexture(2, "assets/textures/Coralie.png", "Mur colore");
    auto big_bricks_texture = texture_db.loadNewTexture(3, "assets/textures/BigBricksTexture20.png", "Mur briques blanc");
    auto ground2_texture = texture_db.loadNewTexture(4, "assets/textures/Ground2.png", "Sol briques blanc");
    auto chest_texture = texture_db.loadNewTexture(5, "assets/textures/ChestSprite.png", "Tresor");
    auto tree_texture = texture_db.loadNewTexture(6, "assets/textures/TreeSprite.png", "Arbre");
    auto small_bunny_texture = texture_db.loadNewTexture(7, "assets/textures/BunnySprite.png", "Petit lapin");
    auto big_bunny_texture = texture_db.loadNewTexture(8, "assets/textures/Bunny2.png", "Gros lapin", 200, 200);
    auto bunny_chest_texture = texture_db.loadNewTexture(9, "assets/textures/BunnyChest.png", "Tresor lapin", 400, 250);
    auto crayon_texture = texture_db.loadNewTexture(10, "assets/textures/crayon.png", "Plaine cire");

    SpriteDb sprite_db;
    auto chest_sprite = sprite_db.addElement({ 1, chest_texture, "Chest", {0}, {0}, {0}, {0} });
    auto tree_sprite = sprite_db.addElement({ 2, tree_texture, "Tree", {0}, {0}, {0}, {0} });
    auto small_bunny_sprite = sprite_db.addElement({ 3, small_bunny_texture, "Small bunny", {0}, {0}, {0}, {0} });
    auto big_bunny_sprite = sprite_db.addElement({ 4, big_bunny_texture, "Big bunny",  {0, 1, 0, 2}, {3, 4, 3, 5}, {6, 7, 8, 7}, {-6, -7, -8, -7} });
    auto bunny_chest_sprite = sprite_db.addElement({ 5, bunny_chest_texture, "Bunny chest", {0}, {1}, {2}, {-2} });


    EntityTemplateDb template_db;
    auto chest_template = template_db.addElement({ 1, chest_sprite, "Chest", MovementType::STATIC, CollisionType::PICKABLE, InteractionType::NONE, 105.0f, 83.0f});
    auto tree_template = template_db.addElement({ 2, tree_sprite, "Tree", MovementType::STATIC, CollisionType::NONE, InteractionType::NONE, 100.0f, 300.0f});
    auto small_bunny_template = template_db.addElement({ 3, small_bunny_sprite, "Small bunny", MovementType::FOLLOW, CollisionType::PICKABLE, InteractionType::NONE, 50.0f});
    auto big_bunny_template = template_db.addElement({ 4, big_bunny_sprite, "Big bunny", MovementType::STATIC, CollisionType::NONE, InteractionType::NONE, 200.0f, 200.0f});
    auto bunny_chest_template = template_db.addElement({ 5, bunny_chest_sprite, "Bunny chest", MovementType::STATIC, CollisionType::PICKABLE, InteractionType::NONE, 400.0f, 250.0f});

    Databases db(ground_db, wall_db, texture_db, template_db, sprite_db);

    //Labyrinth test = Labyrinth(30, 30);
    Labyrinth test = Labyrinth::buildTriangleLabyrinth(3);
    ShallowEntity chest1 = ShallowEntity( 1, chest_template, 5, 5, CardinalDirection::NORTH);
    ShallowEntity chest2 = ShallowEntity( 2, chest_template, 7, 5, CardinalDirection::NORTH);
    ShallowEntity tree1 = ShallowEntity( 3, tree_template, 2, 0, CardinalDirection::NORTH, true, 0.75f, 0.75f);
    ShallowEntity tree2 = ShallowEntity( 4, tree_template, 2, 0, CardinalDirection::NORTH, true, 0.25f, 0.75f);
    //ShallowEntity bunny1 = ShallowEntity(5, small_bunny_template, 0, 1, CardinalDirection::NORTH);
    //ShallowEntity bunny2 = ShallowEntity(6, small_bunny_template, 1, 0, CardinalDirection::NORTH);
    ShallowEntity bunny3 = ShallowEntity(7, big_bunny_template, 2, 0, CardinalDirection::NORTH);
    ShallowEntity bunny_chest = ShallowEntity(8, bunny_chest_template, 2, 2, CardinalDirection::SOUTH);
    ShallowEntity bunny4 = ShallowEntity(9, big_bunny_template, 2, 0, CardinalDirection::NORTH);

    //test.getEntityManager().addEntity(chest1);
    //test.getEntityManager().addEntity(chest2);
    test.getEntityManager().addEntity(tree1);
    test.getEntityManager().addEntity(tree2);
    //test.getEntityManager().addEntity(bunny1);
    //test.getEntityManager().addEntity(bunny2);
    test.getEntityManager().addEntity(bunny3);
    //test.getEntityManager().addEntity(bunny_chest);
    test.getEntityManager().addEntity(bunny4);

    //test.loadFromFile("current.labyrinth");
    //Runner runner = Runner(test, db);
    LabyrinthEditView lve = LabyrinthEditView(test, db);
    bool closed = false;
    sf::Clock fps_clock;
    while (!closed)
    {
        //std::cout << test.printToString();
        closed = lve.processEvents();
        //closed = runner.processEvents() || closed;
        //runner.render();
        lve.render();
        //std::cout << "Done with frame.";
        int frame_time = fps_clock.getElapsedTime().asMilliseconds();
        if (frame_time < 1000 / 30) {
            std::this_thread::sleep_for(std::chrono::milliseconds((1000 / 30) - frame_time));
        }

    }
}