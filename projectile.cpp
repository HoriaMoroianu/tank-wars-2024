#include "lab_m1/tank-wars-2024/projectile.h"
#include "lab_m1/tank-wars-2024/tank_wars.h"

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

bool Projectile::checkCollision()
{
	auto coords = LocateOnTerrain(pos);
	glm::vec2 A = coords.first;
	glm::vec2 B = coords.second;
	glm::vec2 I{};

	float t = (pos.x - A.x) / (B.x - A.x);
	I.y = A.y + t * (B.y - A.y);
	I.x = pos.x;

	if ((pos.y - I.y) >= projectileYTol) {
		return false;
	}

	for (int x = A.x - blastRadius; x < B.x + blastRadius; x += terrainStep) {
		if (x < 0 || x >= TankWars::screenSize.x) {
			continue;
		}

		float temp = sqrt(blastRadius * blastRadius - (x - I.x) * (x - I.x));
		if (std::isnan(temp)) {
			continue;
		}
		float y = -temp + I.y;

		float* height = TankWars::heightMap[x / terrainStep];
		if (y < *height) {
			*height = y;
		}
	}
	return true;
}

glm::mat3 Projectile::getModelMatrix()
{
	return transform2D::Translate(pos.x, pos.y) * transform2D::Scale(size);
}