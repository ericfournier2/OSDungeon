#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib> 

#include "common.h"
#include "labyrinth_background.h"
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
    auto soleil_paint_texture = texture_db.loadNewTexture(11, "assets/textures/soleil paint.png", "Soleil paint", 200, 200);
    auto nuage_paint_texture = texture_db.loadNewTexture(12, "assets/textures/nuage paint.png", "Nuage paint", 200, 150);
    auto montagne_paint_texture = texture_db.loadNewTexture(13, "assets/textures/montagne paint.png", "Montagne paint");
    auto bleu_degrade_texture = texture_db.loadNewTexture(14, "assets/textures/bleu degrade.png", "Bleu degrade");
    auto soleil_cire_texture = texture_db.loadNewTexture(15, "assets/textures/soleil cire.png", "Soleil cire", 212, 222);
    auto nuage_cire_texture = texture_db.loadNewTexture(16, "assets/textures/nuage cire.png", "Nuage cire", 233, 152);
    auto montagne_cire_texture = texture_db.loadNewTexture(17, "assets/textures/montagne cire.png", "Montagne cire");
    auto colline_cire_1_texture = texture_db.loadNewTexture(18, "assets/textures/colline cire 1.png", "Colline cire 1");
    auto colline_cire_2_texture = texture_db.loadNewTexture(19, "assets/textures/colline cire 2.png", "Colline cire 2");
    auto ciel_cire_texture = texture_db.loadNewTexture(20, "assets/textures/ciel cire 2.png", "Ciel cire");
    auto mur_foret_test_texture = texture_db.loadNewTexture(21, "assets/textures/Mur foret cire.png", "Mur foret cire");
    auto bg_plaine_texture = texture_db.loadNewTexture(22, "assets/textures/bg plaine cire.png", "Plaine cire");
    auto animated_flower_texture = texture_db.loadNewTexture(23, "assets/textures/Fleur buisson cire.png", "Animated flower", 107, 110);
    auto fc_front = texture_db.loadNewTexture(24, "assets/textures/fc-front.png", "fc-front");
    auto fc_partial = texture_db.loadNewTexture(25, "assets/textures/fc-partial.png", "fc-partial");
    auto fc_depth_0 = texture_db.loadNewTexture(26, "assets/textures/fc-depth-0.png", "fc-depth-0");
    auto fc_depth_1 = texture_db.loadNewTexture(27, "assets/textures/fc-depth-1.png", "fc-depth-1");

    GroundDb ground_db;
    //ground_db.addElement(GroundInfo({ 0, sf::Color::White, 0 }));
    ground_db.addElement(GroundInfo({ 1, sf::Color::White, sf::Color::Blue, bg_plaine_texture, false }));
    ground_db.addElement(GroundInfo({ 2, sf::Color::Green, sf::Color::Blue, 4, false }));
    ground_db.addElement(GroundInfo({ 3, sf::Color::Red, sf::Color::Blue, 4, false }));
    ground_db.addElement(GroundInfo({ 4, sf::Color::White, sf::Color::White, 10, false }));

    WallDb wall_db;
    //wall_db.addElement(WallInfo({ 0, sf::Color::White, 0 }));
    wall_db.addElement(WallInfo({ 1,
                                  sf::Color::White,
                                  {fc_front, {0}},
                                  {fc_partial, {0}},
                                  { {0, {fc_depth_0, {0}}},
                                    {1, {fc_depth_1, {0}}}
                                  }
                                }));
    //wall_db.addElement(WallInfo({ 2, sf::Color::Green, 3 }));
    //wall_db.addElement(WallInfo({ 3, sf::Color::Red, 3 }));


    SpriteDb sprite_db;
    auto chest_sprite = sprite_db.addElement({ 1, chest_texture, "Chest", {0}, {0}, {0}, {0} });
    auto tree_sprite = sprite_db.addElement({ 2, tree_texture, "Tree", {0}, {0}, {0}, {0} });
    auto small_bunny_sprite = sprite_db.addElement({ 3, small_bunny_texture, "Small bunny", {0}, {0}, {0}, {0} });
    auto big_bunny_sprite = sprite_db.addElement({ 4, big_bunny_texture, "Big bunny",  {0, 1, 0, 2}, {3, 4, 3, 5}, {6, 7, 8, 7}, {-6, -7, -8, -7} });
    auto bunny_chest_sprite = sprite_db.addElement({ 5, bunny_chest_texture, "Bunny chest", {0}, {1}, {2}, {-2} });
    auto soleil_paint_sprite = sprite_db.addElement({ 6, soleil_paint_texture, "Soleil paint", {0, 1, 2}, {0, 1, 2}, {0, 1, 2}, {0, 1, 2} });
    auto nuage_paint_sprite = sprite_db.addElement({ 7, nuage_paint_texture, "Nuage paint", {0, 1, 2}, {0, 1, 2}, {0, 1, 2}, {0, 1, 2} });
    auto montagne_paint_sprite = sprite_db.addElement({ 8, montagne_paint_texture, "Montagne paint", {0}, {0}, {0}, {0} });
    auto soleil_cire_sprite = sprite_db.addElement({ 9, soleil_cire_texture, "Soleil cire", {0, 1, 2}, {0, 1, 2}, {0, 1, 2}, {0, 1, 2} });
    auto nuage_cire_sprite = sprite_db.addElement({ 10, nuage_cire_texture, "Nuage cire", {0, 1, 2}, {0, 1, 2}, {0, 1, 2}, {0, 1, 2} });
    auto montagne_cire_sprite = sprite_db.addElement({ 11, montagne_cire_texture, "Montagne cire", {0}, {0}, {0}, {0} });
    auto colline_cire_1_sprite = sprite_db.addElement({ 12, colline_cire_1_texture, "Colline cire 1", {0}, {0}, {0}, {0} });
    auto colline_cire_2_sprite = sprite_db.addElement({ 13, colline_cire_2_texture, "Colline cire 2", {0}, {0}, {0}, {0} });
    auto animated_flower_sprite = sprite_db.addElement({ 14, animated_flower_texture, "Animated flower",  {0, 1, 2, 1}, {3, 4, 5, 4}, {6, 7, 8, 7}, {-6, -7, -8, -7} });
    auto animated_shrub_sprite = sprite_db.addElement({ 15, animated_flower_texture, "Animated shrub",  {9, 10, 11}, {9, 10, 11}, {9, 10, 11}, {9, 10, 11} });


    EntityTemplateDb template_db;
    auto chest_template = template_db.addElement({ 1, chest_sprite, "Chest", MovementType::STATIC, CollisionType::PICKABLE, InteractionType::NONE, 105.0f, 83.0f});
    auto tree_template = template_db.addElement({ 2, tree_sprite, "Tree", MovementType::STATIC, CollisionType::NONE, InteractionType::NONE, 100.0f, 300.0f});
    auto small_bunny_template = template_db.addElement({ 3, small_bunny_sprite, "Small bunny", MovementType::FOLLOW, CollisionType::PICKABLE, InteractionType::NONE, 50.0f});
    auto big_bunny_template = template_db.addElement({ 4, big_bunny_sprite, "Big bunny", MovementType::STATIC, CollisionType::NONE, InteractionType::NONE, 200.0f, 200.0f, true });
    auto bunny_chest_template = template_db.addElement({ 5, bunny_chest_sprite, "Bunny chest", MovementType::STATIC, CollisionType::PICKABLE, InteractionType::NONE, 400.0f, 250.0f});
    auto flower_template = template_db.addElement({ 6, animated_flower_sprite, "Flower", MovementType::STATIC, CollisionType::NONE, InteractionType::NONE, 75.0f, 75.0f });
    auto shrub_template = template_db.addElement({ 7, animated_shrub_sprite, "Shrub", MovementType::STATIC, CollisionType::NONE, InteractionType::NONE, 75.0f, 75.0f });


    Databases db(ground_db, wall_db, texture_db, template_db, sprite_db);

    BackgroundEntity background_sun({ "Soleil", soleil_cire_sprite, 0.0f, 0.0f, {1200.0f, 10.0f}, 1.0f, 1 });
    BackgroundEntity background_mountain({ "Montagne", montagne_cire_sprite, 0.0f, 0.0f, {300.0f, 50.0f}, 1.0f, 3 });
    BackgroundEntity background_cloud_1({ "Nuage 1", nuage_cire_sprite, 10.0f, 0.0f, {50.0f, 50.0f}, 1.0f, 5 });
    BackgroundEntity background_cloud_2({ "Nuage 2", nuage_cire_sprite, 5.0f, 0.0f, {600.0f, 100.0f}, 0.5f, 2 });
    BackgroundEntity background_hill_1({ "Colline 1", colline_cire_1_sprite, 0.0f, 0.0f, {50.0f, 175.0f}, 1.0f, 4 });
    BackgroundEntity background_hill_2({ "Colline 2", colline_cire_2_sprite, 0.0f, 0.0f, {900.0f, 175.0f}, 1.0f, 4 });



    LabyrinthBackground background;
    background.texture = ciel_cire_texture;
    background.entities.push_back(background_sun);
    background.entities.push_back(background_mountain);
    background.entities.push_back(background_cloud_1);
    background.entities.push_back(background_cloud_2);
    background.entities.push_back(background_hill_1);
    background.entities.push_back(background_hill_2);


    //Labyrinth test = Labyrinth(30, 30);
    Labyrinth test = Labyrinth::buildTriangleLabyrinth(5, &db);
    EntityState chest1 = EntityState( 1, chest_template, 5, 5, CardinalDirection::NORTH);
    EntityState chest2 = EntityState( 2, chest_template, 7, 5, CardinalDirection::NORTH);
    EntityState tree1 = EntityState( 3, tree_template, 2, 0, CardinalDirection::NORTH, true, 0.75f, 0.75f);
    EntityState tree2 = EntityState( 4, tree_template, 2, 0, CardinalDirection::NORTH, true, 0.25f, 0.75f);
    //ShallowEntity bunny1 = ShallowEntity(5, small_bunny_template, 0, 1, CardinalDirection::NORTH);
    //ShallowEntity bunny2 = ShallowEntity(6, small_bunny_template, 1, 0, CardinalDirection::NORTH);
    EntityState bunny3 = EntityState(7, big_bunny_template, 2, 0, CardinalDirection::NORTH);
    EntityState bunny_chest = EntityState(8, bunny_chest_template, 2, 2, CardinalDirection::SOUTH);
    EntityState bunny4 = EntityState(9, big_bunny_template, 2, 0, CardinalDirection::NORTH);
    EntityState shrub_1 = EntityState(10, shrub_template, 4, 0, CardinalDirection::NORTH, true, 0.75f, 0.75f);
    EntityState shrub_2 = EntityState(11, shrub_template, 4, 0, CardinalDirection::SOUTH, true, 0.25f, 0.75f);
    EntityState flower_1 = EntityState(10, flower_template, 4, 1, CardinalDirection::NORTH, true, 0.33f, 0.58f);
    EntityState flower_2 = EntityState(11, flower_template, 4, 1, CardinalDirection::SOUTH, true, 0.25f, 0.25f);


    //test.getEntityManager().addEntity(chest1);
    //test.getEntityManager().addEntity(chest2);
    //test.getEntityManager().addEntity(tree1);
    //test.getEntityManager().addEntity(tree2);
    //test.getEntityManager().addEntity(bunny1);
    //test.getEntityManager().addEntity(bunny2);
    test.getEntityManager().addEntity(bunny3);
    ////test.getEntityManager().addEntity(bunny_chest);
    //test.getEntityManager().addEntity(bunny4);//
    //test.getEntityManager().addEntity(shrub_1);
    //test.getEntityManager().addEntity(shrub_2);
    //test.getEntityManager().addEntity(flower_1);
    //test.getEntityManager().addEntity(flower_2);

    for (int x = 0; x < test.getSizeX(); ++x) {
        for (int y = 0; y < test.getSizeY(); ++y) {
            int n_entities = rand() % 5;
            for (int e = 0; e < n_entities; ++e) {
                float x_sub = float(rand()) / RAND_MAX;
                float y_sub = float(rand()) / RAND_MAX;
                CardinalDirection d = static_cast<CardinalDirection>(rand() % 4);
                EntityState ent(0, flower_template, x, y, d, true, x_sub, y_sub);
                if (rand() % 2 == 0) {
                    ent.template_id = shrub_template;
                }
              
                test.getEntityManager().addEntity(ent);
            }
        }
    }

    //test.loadFromFile("current.labyrinth");
    //Runner runner = Runner(test, db);
    LabyrinthEditView lve = LabyrinthEditView(test, background, db);
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