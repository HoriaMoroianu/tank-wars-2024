#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tank-wars-2024/transform2D.h"
#include "lab_m1/tank-wars-2024/utils.h"
#include "lab_m1/tank-wars-2024/tank.h"

namespace tw
{
	class Tank;

	class Projectile
	{
	public:
		Projectile(Tank *tank);
		~Projectile();

		void moveProjectile(const float deltaTime);
		glm::mat3 getModelMatrix();

	private:
		Tank *tank;
		glm::vec2 pos;
		glm::vec2 speed;
		glm::mat3 offsetShootPos;

		const float magnitude = 150;
		const float size = 0.2f;
	};
}
