#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tank-wars-2024/transform2D.h"
#include "lab_m1/tank-wars-2024/utils.h"
#include "lab_m1/tank-wars-2024/projectile.h"

namespace tw
{
	class Tank
	{
	public:
		Tank(glm::vec2 pos, glm::vec3 primaryColor, glm::vec3 secondaryColor);
		~Tank();

		std::vector<Mesh*> getTankMeshes();
		glm::mat3 getTransformationMatrix();
		glm::mat3 getProjectileMatrix();
		std::vector<std::pair<std::string, glm::mat3>> getTankParts();
		void moveTank(const float distance);
		void rotateGun(const float angle);

		void shoot();
		void updateProjectiles(const float deltaTime);
		bool hitByProjectile(const glm::vec2 projectilePos, const float projectileSize);

		std::vector<class Projectile>& getProjectiles() { return projectiles; };
		float getGunAngle() const { return noseAngle; };
		float getTankAngle() const { return tankAngle; };
		float getTankSize() const { return tankSize; };
		Mesh* getTrajectoryMesh();

	private:

	private:
		static int instances;
		std::string id;

		int health;
		bool isDead = false;

		glm::vec2 tankPos;
		glm::vec3 primaryColor;
		glm::vec3 secondaryColor;

		float noseAngle;
		float tankSize;
		float tankAngle;

		Mesh* trajectoryMesh;
		std::vector<class Projectile> projectiles;
	};
}
