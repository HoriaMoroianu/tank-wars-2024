#include "lab_m1/tank-wars-2024/projectile.h"

#include <iostream>
using namespace tw;

Projectile::Projectile(Tank *tank) : tank(tank)
{
	offsetShootPos = tank->getProjectileMatrix();
	pos = { offsetShootPos[2][0], offsetShootPos[2][1] };
	float gunAngle = tank->getGunAngle() + M_PI_2;
	speed = glm::vec2{ cos(gunAngle), sin(gunAngle) } * magnitude;
}

Projectile::~Projectile()
{
}

void Projectile::moveProjectile(const float deltaTime)
{
	pos += speed * deltaTime;
	speed -= gravity * deltaTime;
}

glm::mat3 Projectile::getModelMatrix()
{
	std::cout << pos.x << " " << pos.y << std::endl;
	return transform2D::Translate(pos.x, pos.y);
}