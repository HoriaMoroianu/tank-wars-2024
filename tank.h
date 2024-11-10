#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tank-wars-2024/transform2D.h"
#include "lab_m1/tank-wars-2024/utils.h"

namespace tw
{
	class Tank
	{
	public:
		Tank();
		~Tank();

		std::vector<Mesh*> getTankMeshes();
		glm::mat3 getTransformationMatrix();
		glm::mat3 getProjectileMatrix();
		std::vector<std::pair<std::string, glm::mat3>> getTankParts();
		void moveTank(const float distance);
		void rotateGun(const float angle);

		float noseAngle;
	private:

	private:
		static int instances;
		std::string id;

		glm::vec2 tankPos;	// TODO put in constructor + colors
		float tankSize;
		float tankAngle;
	};
}
