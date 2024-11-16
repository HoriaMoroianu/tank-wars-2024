#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tank-wars-2024/tank.h"

namespace tw
{
	const glm::vec3 colorBlack = { 0, 0, 0 };
	const glm::vec3 colorWhite = { 1, 1, 1 };
	const glm::vec3 colorBrown = { 0.702f, 0.596f, 0.373f };
	const glm::vec3 colorDarkBrown = { 0.42f, 0.365f, 0.251f };
	const glm::vec3 colorBlueSky = { 0.53f, 0.81f, 0.98f };
	const glm::vec3 colorGreenGrass = { 0.3f, 0.55f, 0.03f };
	const glm::vec3 colorMillitaryGreen = { 0.3f, 0.36f, 0.25f };
	const glm::vec3 colorDarkGreen = { 0.1f, 0.2f, 0.1f };

	const glm::vec2 gravity = { 0, 90 };

	const float tankAngleTol = M_PI / 6;
	const float projectileYTol = 0.1f;
	
	const float terrainThreshold = 0.1f;
	const float terrainDiffEpsilon = 15.f;

	const int terrainStep = 5;
	const int blastRadius = 10 * terrainStep;

	const float magnitude = 150;

	float TerrainGenerator(const int x);
	void SimulateLandslide(const float deltaTime);
	Mesh* CreateTrapeze(const std::string name, const glm::vec3 color);
	Mesh* CreateCircle(const std::string name, const glm::vec3 color, const int subdivisions = 15);
	Mesh* CreateSquare(const std::string name, const glm::vec3 color, bool fill = false);
	std::pair<std::vector<VertexFormat>, std::vector<unsigned int>> UpdateTrajectory(Mesh* mesh, class Tank* tank);
	Mesh* CreateTrajectory(const std::string name, class Tank* tank);
	std::pair<glm::vec2, glm::vec2> LocateOnTerrain(const glm::vec2 pos);
}