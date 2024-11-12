#include "lab_m1/tank-wars-2024/projectile.h"

#include <iostream>
using namespace tw;

Projectile::Projectile(Tank *tank) : tank(tank)
{
	glm::mat3 tankTansform = tank->getProjectileMatrix();
	pos = { tankTansform[2][0], tankTansform[2][1] };

	float gunAngle = tank->getGunAngle() + tank->getTankAngle() + M_PI_2;
	speed = glm::vec2{ cos(gunAngle), sin(gunAngle) } * magnitude;
	size = 0.2f * tank->getTankSize();
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
	return transform2D::Translate(pos.x, pos.y) * transform2D::Scale(size);
}