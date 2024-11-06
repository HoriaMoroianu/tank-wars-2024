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

		void Update();

	private:
		std::unordered_map<std::string, glm::mat3> CreateModel
			(const float tPosX, const float tPosY, const float tScale, const float noseAngle);

	private:
		std::vector<Mesh*> tankMeshes;
		std::unordered_map<std::string, glm::mat3> tankParts;
		glm::vec2 tankPos;
		float tankScale;
		float tankAngle;
		float noseAngle;
	};
}