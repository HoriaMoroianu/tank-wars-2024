#include "lab_m1/tank-wars-2024/tank.h"

#include <iostream>
using namespace tw;

int Tank::instances = 0;

Tank::Tank()
{
	Tank::instances++;
	id = std::to_string(Tank::instances);
    tankPos = { 600, 150 };
    tankSize = 50;
    noseAngle = RADIANS(-75);
	tankAngle = 0;
}

Tank::~Tank()
{
}

std::vector<Mesh*> Tank::getTankMeshes()
{
    // Order matters! First in list is drawn in front
    std::vector<Mesh*> meshes{};
    meshes.push_back(CreateTrapeze("tank_body" + id, colorBrown));
	meshes.push_back(CreateTrapeze("tank_base" + id, colorDarkBrown));
	meshes.push_back(CreateCircle("tank_head" + id, colorBrown, 31));
	meshes.push_back(CreateSquare("tank_nose" + id, colorBlack));
    return meshes;
}

std::vector<std::pair<std::string, glm::mat3>> Tank::getTankParts()
{
    std::vector<std::pair<std::string, glm::mat3>> tankParts{};

	glm::mat3 tankTranslation = transform2D::Translate(tankPos.x, tankPos.y);
    glm::mat3 tankScale = transform2D::Scale(tankSize);
    glm::mat3 transformationMatrix = tankTranslation * tankScale;

    // Tank Body
    glm::mat3 bodyMatrix = transformationMatrix;
    bodyMatrix *= transform2D::Translate(0, 0.33f);
    bodyMatrix *= transform2D::Scale(1.25f, 0.7f);
    tankParts.push_back({ "tank_body" + id, bodyMatrix });

    // Tank Base
    glm::mat3 baseMatrix = transformationMatrix;
    baseMatrix *= transform2D::Translate(0, 0.125f);
    baseMatrix *= transform2D::Scale(1, 0.5f);
    baseMatrix *= transform2D::Rotate(M_PI);
	tankParts.push_back({ "tank_base" + id, baseMatrix });

    // Tank Head
    glm::mat3 headMatrix = transformationMatrix;
    headMatrix *= transform2D::Translate(0, 0.5f);
    headMatrix *= transform2D::Scale(0.5f);
	tankParts.push_back({ "tank_head" + id, headMatrix });

    // Tank Nose
    glm::mat3 noseMatrix = transformationMatrix;
    noseMatrix *= transform2D::Translate(0, 0.5f);
    noseMatrix *= transform2D::Rotate(noseAngle);
    noseMatrix *= transform2D::Translate(0, 0.45f);
    noseMatrix *= transform2D::Scale(0.12f, 0.9f);
    noseMatrix *= transform2D::Translate(0, 0.5f);
	tankParts.push_back({ "tank_nose" + id, noseMatrix });

	return tankParts;
}

void Tank::moveTank(const float distance)
{
	// TODO no magic numbers
    tankPos.x = clamp(tankPos.x + distance, 0.0f, 1200.0f);
}

void Tank::rotateGun(const float angle)
{
	noseAngle = tw::clamp(noseAngle + angle, RADIANS(-75), RADIANS(75));
}
