#pragma once

#include "components/simple_scene.h"

namespace tw
{
	const glm::vec3 colorBlack = { 0, 0, 0 };
	const glm::vec3 colorBrown = { 0.702f, 0.596f, 0.373f };
	const glm::vec3 colorDarkBrown = { 0.42f, 0.365f, 0.251f };

	const glm::vec3 colorBlueSky = { 0.53f, 0.81f, 0.98f };
	const glm::vec3 colorGreenGrass = { 0.3f, 0.55f, 0.03f };

	float TerrainGenerator(const int x);
	Mesh* CreateTrapeze(const std::string name, const glm::vec3 color);
	Mesh* CreateCircle(const std::string name, const glm::vec3 color, const int subdivisions = 15);
	Mesh* CreateSquare(const std::string name, const glm::vec3 color);
}