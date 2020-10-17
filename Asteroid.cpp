#include "Asteroid.hpp"
#include <stdlib.h>
#include <math.h>

bool Asteroid::isTouchingBullet(Bullet& bullet)
{
    // Checking if the bullet touches the asteroid by comparing the distance to the sum of the radiuses.
    float diffX = bullet.x - x;
    float diffY = bullet.y - y;
    float distance = sqrt(diffX * diffX + diffY * diffY);
    return distance < size * 50 + 10;
}

void Asteroid::setRandomSpeed()
{
	// The smaller the size, the higher the speed.
    speedX = (float) rand() / RAND_MAX * 5 / size - (5 / (size * 2));
    speedY = (float) rand() / RAND_MAX * 5 / size - (5 / (size * 2));
    rotationSpeed = (float) rand() / RAND_MAX * 5 / size - (5 / (size * 2));
}


