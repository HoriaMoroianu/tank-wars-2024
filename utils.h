#pragma once

#include "components/simple_scene.h"

namespace tw
{
	const glm::vec3 colorBlack = { 0, 0, 0 };
	const glm::vec3 colorBrown = { 0.6f, 0.3f, 0.1f };
	const glm::vec3 colorDarkBrown = { 0.4f, 0.2f, 0.1f };

	Mesh* CreateTrapeze(const std::string name, const glm::vec3 color);
	Mesh* CreateCircle(const std::string name, const glm::vec3 color, const int subdivisions = 15);
	Mesh* CreateSquare(const std::string name, const glm::vec3 color);
}