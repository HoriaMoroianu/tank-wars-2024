#include "lab_m1/tank-wars-2024/tank.h"


using namespace tw;

Tank::Tank()
{
	// Order matters! First in list is drawn in front
    tankMeshes.push_back(CreateTrapeze("tank_body", colorBrown));
    tankMeshes.push_back(CreateTrapeze("tank_base", colorDarkBrown));
    tankMeshes.push_back(CreateCircle("tank_head", colorBlack, 31));
    tankMeshes.push_back(CreateSquare("tank_nose", colorBlack));
}

Tank::~Tank()
{
}

std::unordered_map<std::string, glm::mat3> Tank::CreateModel
    (const float tPosX, const float tPosY, const float tScale, const float noseAngle)
{
	std::unordered_map<std::string, glm::mat3> tankParts;

	glm::mat3 tankTranslation = transform2D::Translate(tPosX, tPosY);
    glm::mat3 tankScale = transform2D::Scale(tScale);
    glm::mat3 transformationMatrix = tankTranslation * tankScale;

    // Tank Body
    glm::mat3 bodyMatrix = transformationMatrix;
    bodyMatrix *= transform2D::Translate(0, 0.33f);
    bodyMatrix *= transform2D::Scale(1.25f, 0.7f);
	tankParts["tank_body"] = bodyMatrix;

    // Tank Base
    glm::mat3 baseMatrix = transformationMatrix;
    baseMatrix *= transform2D::Translate(0, 0.125f);
    baseMatrix *= transform2D::Scale(1, 0.5f);
    baseMatrix *= transform2D::Rotate(M_PI);
	tankParts["tank_base"] = baseMatrix;

    // Tank Head
    glm::mat3 headMatrix = transformationMatrix;
    headMatrix *= transform2D::Translate(0, 0.5f);
    headMatrix *= transform2D::Scale(0.5f);
	tankParts["tank_head"] = headMatrix;

    // Tank Nose
    glm::mat3 noseMatrix = transformationMatrix;
    noseMatrix *= transform2D::Translate(0, 0.5f);
    noseMatrix *= transform2D::Rotate(noseAngle);
    noseMatrix *= transform2D::Translate(0, 0.5f);
    noseMatrix *= transform2D::Scale(0.12f, 0.85f);
    noseMatrix *= transform2D::Translate(0, 0.5f);
	tankParts["tank_nose"] = noseMatrix;

	return tankParts;
}

void Tank::Update()
{
	auto tankParts = CreateModel(300, 300, 150, -M_PI / 2.5f);

}
