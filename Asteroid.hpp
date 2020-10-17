#pragma once

#include "Bullet.hpp"

// This class represents an asteroid, it's speed, angle and size.
class Asteroid
{
public:
    float x, y, angle;
    int size;
    float speedX, speedY, rotationSpeed;
    bool isTouchingBullet(Bullet& bullet); // Quick method to check if the asteroid touches a bullet.
    void setRandomSpeed(); // Quick methode to make the asteroid travel and spin at random speeds.
};


