/**
Sumayyah Alhydary
CS207 Final Project
This is a simple asteroids game written in C++ using SFML and other libraries.
**/

#include "Game.hpp"

int main()
{
    // Creating a Game object, and destroying it once it returns from it's start() method.
    Game* game = new Game();
    game->start();
    delete game;
    return 0;
}

