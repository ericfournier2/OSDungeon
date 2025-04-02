#include <SFML/Graphics.hpp>
#include <iostream>
#include "labyrinth.h";
#include "labyrinth_view.h"
#include "labyrinth_edit_view.h"

void labyrinthTestSymmetric();
void labyrinthTestAssymmetric();
Labyrinth getTestLabyrinth();

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 400, 300 }), "Maze!");

    sf::Font font;
    font.openFromFile("LEMONMILK - Regular.otf");

    Labyrinth test = getTestLabyrinth();
    LabyrinthView lv = LabyrinthView(test, window, font, 400, 300);
    LabyrinthEditView lve = LabyrinthEditView(window, test);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
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
            }

        }

        window.clear();
        std::cout << test.printToString();
        //lv.render();
        lve.render();
        window.display();
        std::cout << "Done with frame.";
    }
}