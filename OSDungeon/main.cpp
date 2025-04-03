#include <SFML/Graphics.hpp>
#include <iostream>
#include "labyrinth.h";
#include "labyrinth_view.h"
#include "labyrinth_edit_view.h"

void labyrinthTestSymmetric();
void labyrinthTestAssymmetric();
Labyrinth getTestLabyrinth();
void testReadWrite();

int main()
{
    //testReadWrite();
    //sf::RenderWindow window(sf::VideoMode({ 400, 300 }), "Maze!");

    sf::Font font;
    font.openFromFile("LEMONMILK - Regular.otf");

    //Labyrinth test = getTestLabyrinth();
    Labyrinth test = Labyrinth(10, 10);
    LabyrinthView lv = LabyrinthView(test, font, 400, 300);
    LabyrinthEditView lve = LabyrinthEditView(test);
    bool closed = false;
    while (!closed)
    {
        /*while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Left) {
                    test.turnPovRel(LEFT);
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::Up) {
                    test.advance();
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::Right) {
                    test.turnPovRel(RIGHT);
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::Down) {
                    test.moveBack();
                }
            } else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Right)
                {
                    std::cout << "the right button was pressed" << std::endl;
                    std::cout << "mouse x: " << mouseButtonPressed->position.x << std::endl;
                    std::cout << "mouse y: " << mouseButtonPressed->position.y << std::endl;
                }
            }

        }
        */
        //window.clear();
        std::cout << test.printToString();
        closed = lve.processEvents();
        closed = lv.processEvents() | closed;
        lv.render();
        lve.render();
        //window.display();
        std::cout << "Done with frame.";
    }
}