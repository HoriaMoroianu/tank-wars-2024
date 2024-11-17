#include "lab_m1/tank-wars-2024/tank.h"
#include "lab_m1/tank-wars-2024/tank_wars.h"

#include <iostream>
using namespace tw;

int Tank::instances = 0;

Tank::Tank(glm::vec2 pos, glm::vec3 primaryColor, glm::vec3 secondaryColor)
{
    Tank::instances++;
    id = std::to_string(Tank::instances);

	this->primaryColor = primaryColor;
	this->secondaryColor = secondaryColor;
	this->tankPos = pos;

    tankSize = 30;
    noseAngle = 0;
    tankAngle = 1;
	health = 100;
    trajectoryMesh = CreateTrajectory("trajectory" + id, this);
}

Tank::~Tank()
{
}

std::vector<Mesh*> Tank::getTankMeshes()
{
    // Order matters! First in list is drawn in front
    std::vector<Mesh*> meshes{};
    meshes.push_back(CreateTrapeze("tank_body" + id, primaryColor));
	meshes.push_back(CreateTrapeze("tank_base" + id, secondaryColor));
	meshes.push_back(CreateCircle("tank_head" + id, primaryColor, 31));
    meshes.push_back(CreateSquare("tank_nose" + id, colorBlack, true));
    meshes.push_back(CreateSquare("tank_hp_bar" + id, colorWhite));
    meshes.push_back(CreateSquare("tank_hp" + id, colorWhite, true));
    return meshes;
}

glm::mat3 Tank::getTransformationMatrix()
{
    glm::mat3 tankTranslation = transform2D::Translate(tankPos.x, tankPos.y);
    glm::mat3 tankScale = transform2D::Scale(tankSize);
    glm::mat3 tankRotation = transform2D::Rotate(tankAngle);
	return tankTranslation * tankRotation * tankScale;
}

glm::mat3 Tank::getProjectileMatrix()
{
    glm::mat3 projectileMatrix = getTransformationMatrix();
    projectileMatrix *= transform2D::Translate(0, 0.5f);
    projectileMatrix *= transform2D::Rotate(noseAngle);
    projectileMatrix *= transform2D::Translate(0, 1.5f);
	return projectileMatrix;
}

std::vector<std::pair<std::string, glm::mat3>> Tank::getTankParts()
{
    if (isDead) {
        return {};
    }
    std::vector<std::pair<std::string, glm::mat3>> tankParts{};
    glm::mat3 transformationMatrix = getTransformationMatrix();

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

	// Tank HP Bar
    glm::mat3 barMatrix = transformationMatrix;
	barMatrix *= transform2D::Translate(0, 1.5f);
	barMatrix *= transform2D::Scale(2.f, 0.3f);
    tankParts.push_back({ "tank_hp_bar" + id, barMatrix });

    // Tank HP
    glm::mat3 hpMatrix = transformationMatrix;
    hpMatrix *= transform2D::Translate(-1.f, 1.5f);
    hpMatrix *= transform2D::Scale(2.f * health / 100, 0.3f);
    hpMatrix *= transform2D::Translate(0.5f, 0);
    tankParts.push_back({ "tank_hp" + id, hpMatrix });

	return tankParts;
}

Mesh* Tank::getTrajectoryMesh()
{
	if (isDead) {
		return nullptr;
	}

    auto trajectoryData = UpdateTrajectory(trajectoryMesh, this);
    trajectoryMesh->InitFromData(trajectoryData.first, trajectoryData.second);
	return trajectoryMesh;
}

void Tank::moveTank(const float distance)
{
    tankPos.x = glm::clamp(tankPos.x + distance, 0.0f, TankWars::screenSize.x);
    
	auto coords = LocateOnTerrain(tankPos);
	glm::vec2 A = coords.first;
	glm::vec2 B = coords.second;

	float t = (tankPos.x - A.x) / (B.x - A.x);
	tankPos.y = A.y + t * (B.y - A.y);

	float targetAngle = atan2(B.y - A.y, B.x - A.x);
	// Snap if angle is too steep, else interpolate
    tankAngle = (abs(tankAngle - targetAngle) > tankAngleTol) ? targetAngle :
        glm::mix(tankAngle, targetAngle, 0.8f);

    if (tankPos.y < 0) {
        tankPos.y = 0;
		tankAngle = 0;
    }
}

void Tank::rotateGun(const float angle)
{
	noseAngle = glm::clamp(noseAngle + angle, RADIANS(-80), RADIANS(80));
}

void Tank::shoot()
{
    if (isDead) {
		return;
    }
	projectiles.push_back(Projectile(this));
}

void Tank::updateProjectiles(const float deltaTime)
{
    std::vector<Projectile> newProjectiles {};
    for (auto& projectile : projectiles) {
        projectile.moveProjectile(deltaTime);

		glm::vec2 pos = projectile.getPos();
		float size = projectile.getSize();

        if (pos.x < 0 || pos.y < 0 || pos.x > TankWars::screenSize.x || 
            TankWars::GetInstance()->checkTankCollision(pos, size) ||
            projectile.terrainCollision()) {
			projectile.~Projectile();
			continue;
        }
		newProjectiles.push_back(projectile);
    }
	projectiles = std::move(newProjectiles);
}

bool Tank::hitByProjectile(const glm::vec2 projectilePos, const float projectileSize)
{
	if (isDead) {
		return false;
	}

	float dx = tankPos.x - projectilePos.x;
	float dy = tankPos.y - projectilePos.y;
    float d = sqrt(dx * dx + dy * dy);

	if (d > tankSize + projectileSize) {
		return false;
	}
	health -= 20;
	if (health <= 0) {
		isDead = true;
	}
	return true;
}
