#include "Game.hpp"
#include <math.h>
#include <iostream>
using namespace std;

//constructor
Game::Game() {
    window = new sf::RenderWindow(sf::VideoMode(900, 600), "Asteroids", sf::Style::Close);
	// Creating two big hexagonal asteroids.
    asteroids.push_back({
                                10.f, 10.f, 0.f, // pos and angle
                                5, // size
                                1.f, 2.f, 1.f // speeds
                        });
    asteroids.push_back({
                                850.f, 550.f, 0.f, // pos and angle
                                5, // size
                                -1.f, -2.f, 1.f // speeds
                        });

	//asteroids[0].setRandomSpeed();
	//asteroids[1].setRandomSpeed();

    spawn();
}


//Destructors
Game::~Game() {
    delete window; }

void Game::start() {
    while (window->isOpen()) {
        // Polling events
        sf::Event e;
        while (window->pollEvent(e)) //Pop the event on top of the event queue, if any, and return it.
        {
            if (e.type == sf::Event::Closed) {
                window->close();
                //Close the window and destroy all the attached resources
            }
        }

        window->clear();

        // Ticking and rendering the game.
        tick();
        render();

        window->display();

        // We want the game to be running at 60 fps.
        sf::sleep(sf::milliseconds(1000 / 60));
    }
}

void Game::tick() {
    // Check if player died
    if (player.lives == 0) {
        window->close(); }

    // Check if player is colliding an asteroid
    // (using the same technique as in Asteroid::isTouchingBullet(Bullet& bullet)).
    for (Asteroid& a : asteroids) {
        float diffX = a.x - player.x;
        float diffY = a.y - player.y;
        float distance = sqrt(diffX * diffX + diffY * diffY);
        if (distance < 30 + a.size * 50) {
            player.lives --;
            spawn();
        }
    }
    for (Asteroid& a : asteroids) {
        // Spinning and moving the asteroids.
        a.angle += a.rotationSpeed;
        a.x += a.speedX;
        a.y += a.speedY;

        // If asteroids go out of bounds, they are teleported to the other side of the window.
        if (a.y > window->getSize().y + a.size * 50)
            a.y = -a.size * 50 + 1;
        if (a.y < -a.size * 50)
            a.y = window->getSize().y + a.size * 50 - 1;
        if (a.x > window->getSize().x + a.size * 50)
            a.x = -a.size * 50 + 1;
        if (a.x < -a.size * 50)
            a.x = window->getSize().x + a.size * 50 - 1;
    }


    for (Bullet& b : bullets) {
        b.move();
    }

    // Move the player.
    player.x += player.speedX;
    player.y += player.speedY;

    //Decelerate
    player.speedX /= 1.1f;
    player.speedY /= 1.1f;

    // Get key presses.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        player.speedY -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        player.speedY += 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        player.speedX -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        player.speedX += 1.f;
    }

    // Also teleport the player if he goes out of bounds.
    if (player.y > window->getSize().y + 30.f)
        player.y = -30.f + 1;
    if (player.y < -30.f)
        player.y = window->getSize().y + 30.f - 1;
    if (player.x > window->getSize().x + 30.f)
        player.x = -30.f + 1;
    if (player.x < -30.f)
        player.x = window->getSize().x + 30.f - 1;


    // Rotate the player towards the mouse using trigonometry.
    float diffX = sf::Mouse::getPosition(*window).x - player.x;
    float diffY = sf::Mouse::getPosition(*window).y - player.y;
    player.angle = atan(diffY / diffX) * (180 / M_PI); // Converting radians to degrees.
    if (diffX < 0)
        player.angle += 180.f;

    // Make the player shoot bullets.
    if (player.gunCooldown > 0.f) // If playerGunCooldown is bigger than zero, don't shoot.
    {
        player.gunCooldown -= 0.01f;
    }
    else
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            shoot();
            player.gunCooldown = 0.2f; //the smaller the number is the faster the gun can shoot
        }
    }

    // Remove from vector bullets that are out of bounds or that touch asteroids. If a bullet touches
    // an asteroid, split the asteroid in half. If the asteroid is too small, destroy it.
    for (int i = bullets.size() - 1; i > -1; i--) // Iterating through the vector backwards.
    {
        if (
            bullets[i].x > window->getSize().x ||
            bullets[i].x < 0 ||
            bullets[i].y > window->getSize().y ||
            bullets[i].y < 0
        )
        {
            bullets.erase(bullets.begin() + i);
        }
        for (int j = asteroids.size() - 1; j > -1; j--) {
            if (asteroids[j].isTouchingBullet(bullets[i])) {
                asteroids[j].setRandomSpeed();
                if (asteroids[j].size == 1) {
					// If the asteroid size equals one and touches a bullet, destroy it.
                    asteroids.erase(asteroids.begin() + j);
                    bullets.erase(bullets.begin() + i);
                    break;
                }
                else {
					// If the asteroid size is bigger than one, reduce it's size and add an asteroid.
                    asteroids[j].size --;
                    bullets.erase(bullets.begin() + i);
                    Asteroid a = asteroids[j];
                    asteroids[j].setRandomSpeed();
                    a.setRandomSpeed();
                    asteroids.push_back(a);
                    break;
                }
            }
        }
    }
}

void Game::render()
{
    // Render the asteroids.
    sf::CircleShape polygonShape;
    polygonShape.setFillColor(sf::Color::Transparent);
    polygonShape.setOutlineColor(sf::Color::White);
    polygonShape.setOutlineThickness(30.f);
    for (Asteroid& a : asteroids)
    {
        float radius = a.size * 50;
        // Setting the origin to x:radius, y:radius so the asteroid appears to be spinning around his center.
        polygonShape.setOrigin(radius, radius);
        polygonShape.setPosition(a.x, a.y);
        // Asteroids are polygons. We can render them by making a circle and setting a very low point count.
        // The smallest shape will be a triangle, so we always add 2 to the point count.
        polygonShape.setPointCount(a.size + 2);
        polygonShape.setRotation(a.angle);
        polygonShape.setRadius(radius);
        window->draw(polygonShape);
    }
    polygonShape.setOutlineColor(sf::Color::Transparent);

    // Render the bullets
    polygonShape.setFillColor(sf::Color::Red);
    polygonShape.setOrigin(0, 0);
    polygonShape.setRotation(0);
    float radius = 10.f;
    polygonShape.setRadius(radius);
    for (Bullet& b : bullets)
    {
        polygonShape.setPosition(b.x - radius, b.y - radius);
        polygonShape.setPointCount(10);
        window->draw(polygonShape);
    }

    // Render the player.
    polygonShape.setFillColor(sf::Color::Green);
    polygonShape.setOrigin(30, 30);
    polygonShape.setPosition(player.x, player.y);
    polygonShape.setPointCount(3);
    polygonShape.setRadius(30);
    polygonShape.setRotation(player.angle + 90.f);
    window->draw(polygonShape);
}

void Game::spawn()
{
    player.x = window->getSize().x / 2;
    player.y = window->getSize().x / 2;
    player.speedX = 0;
    player.speedY = 0;
    player.angle = 0;

    // When the player spawns, no asteroids should be on the player
    for (int i = asteroids.size() - 1; i > -1; i--)
    {
        float diffX = asteroids[i].x - player.x;
        float diffY = asteroids[i].y - player.y;
        float distance = sqrt(diffX * diffX + diffY * diffY);
        if (distance < 30 + asteroids[i].size * 50)
        {
            asteroids.erase(asteroids.begin() + i);
        }
    }
}

void Game::shoot()
{
    // Make the player shoot a bullet towards the mouse pointer.
    float speedX, speedY;
    // Get the speed of the bullet using trigonometry.
    float radAngle = player.angle * M_PI / 180.f;
    speedX = cos(radAngle) * 10;
    speedY = sin(radAngle) * 10;
    bullets.push_back({player.x, player.y, speedX, speedY});
}