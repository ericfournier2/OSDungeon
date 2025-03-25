#include <SFML/Graphics.hpp>
#include <iostream>
#include "labyrinth.h";
#include "labyrinth_view.h"

void labyrinthTestSymmetric();
void labyrinthTestAssymmetric();
Labyrinth getTestLabyrinth();

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML works!");

    sf::Font font;
    font.openFromFile("LEMONMILK - Regular.otf");

    Labyrinth test = getTestLabyrinth();
    LabyrinthView lv = LabyrinthView(test, window, font, 800, 600);

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
        lv.render();
        window.display();
        std::cout << "Done with frame.";
    }
}