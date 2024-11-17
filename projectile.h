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
		bool terrainCollision();

		glm::mat3 getModelMatrix();
		glm::vec2 getPos() const { return pos; };
		float getSize() const { return size; };

	private:
		Tank *tank;
		glm::vec2 pos;
		glm::vec2 speed;
		float size;
	};
}
