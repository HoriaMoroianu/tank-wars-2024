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
		glm::vec2 getPos() const { return pos; };

	private:
		Tank *tank;
		glm::vec2 pos;
		glm::vec2 speed;
		float size;

		const float magnitude = 150;
	};
}
