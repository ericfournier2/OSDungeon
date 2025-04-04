#include <SFML/Graphics.hpp>
#include <iostream>
#include "labyrinth.h"
#include "labyrinth_view.h"
#include "labyrinth_edit_view.h"

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

    Labyrinth test = Labyrinth(0, 0);
    test.loadFromFile("current.labyrinth");
    LabyrinthView lv = LabyrinthView(test, font, 400, 300);
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