#pragma once

#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "Asteroid.hpp"
#include "Bullet.hpp"
#include "Player.hpp"

class Game
{
public:
    Game();
    ~Game();
    void start();
private:
    Player player;
    sf::RenderWindow* window;
    std::vector<Asteroid> asteroids; // Contains all asteroids.
    std::vector<Bullet> bullets; // Contains all bullets
    void tick(); //check if player died, isTouchingBullet,
    void render();
    void spawn();
    void shoot(); // Shoots a bullet towards the mouse.
};
