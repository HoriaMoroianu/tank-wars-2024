#include "lab_m1/tank-wars-2024/tank_wars.h"
#include <iostream>

using namespace tw;

TankWars* TankWars::instance = nullptr;
std::vector<float*> TankWars::heightMap;
glm::vec2 TankWars::screenSize = glm::vec2(1280, 720);

TankWars::TankWars()
{
	tank1 = nullptr;
	tank2 = nullptr;
}

TankWars::~TankWars()
{
}

void TankWars::CreateTerrain()
{
    for (int i = 0; i < window->GetResolution().x + terrainOffset; i += terrainStep) {
        terrainVertices.push_back(VertexFormat(glm::vec3(i, TerrainGenerator(i), 0), colorGreenGrass));
        terrainVertices.push_back(VertexFormat(glm::vec3(i, -terrainOffset, 0), colorGreenGrass));
    }

	// Mandatory for avoiding memory reallocation
    heightMap.reserve(terrainVertices.size() / 2);

    for (int i = 0; i < terrainVertices.size(); i++) {
        if ((i & 1) == 0) {
            heightMap.push_back(&terrainVertices[i].position.y);
        }
        indices.push_back(i);
    }

    Mesh* terrain = new Mesh("terrain");
    terrain->SetDrawMode(GL_TRIANGLE_STRIP);
    terrain->InitFromData(terrainVertices, indices);
    AddMeshToList(terrain);
}

void TankWars::InitTank(class Tank** tank, glm::vec2 pos, glm::vec3 primaryColor, glm::vec3 secondaryColor)
{
	*tank = new Tank(pos, primaryColor, secondaryColor);
	for (auto& mesh : (*tank)->getTankMeshes()) {
		AddMeshToList(mesh);
	}
	(*tank)->moveTank(0);
}

void TankWars::DrawTankAndProjectiles(const float deltaTime, class Tank* tank)
{
	tank->updateProjectiles(deltaTime);
	for (auto& projectile : tank->getProjectiles()) {
		RenderMesh2D(meshes["projectile"], shaders["VertexColor"], projectile.getModelMatrix());
	}
	tank->moveTank(0);
	auto& tankParts = tank->getTankParts();
	for (auto& part : tankParts) {
		RenderMesh2D(meshes[part.first], shaders["VertexColor"], part.second);
	}
}

bool TankWars::checkTankCollision(glm::vec2 projectilePos, float projectileSize)
{
	return (tank1->hitByProjectile(projectilePos, projectileSize) ||
		tank2->hitByProjectile(projectilePos, projectileSize));
}


void TankWars::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    TankWars::screenSize = glm::vec2(resolution.x, resolution.y);
    
	CreateTerrain();
	InitTank(&tank1, glm::vec2(resolution.x / 3, 0), colorBrown, colorDarkBrown);
	InitTank(&tank2, glm::vec2(resolution.x * 2 / 3, 0), colorMillitaryGreen, colorDarkGreen);
	AddMeshToList(CreateCircle("projectile", colorBlack));
}


void TankWars::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorBlueSky.r, colorBlueSky.g, colorBlueSky.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void TankWars::Update(float deltaTimeSeconds)
{
	// Draw terrain
	SimulateLandslide(deltaTimeSeconds);
	meshes["terrain"]->InitFromData(terrainVertices, indices);
    RenderMesh2D(meshes["terrain"], shaders["VertexColor"], glm::mat3(1.0f));

	// Update tank1
	DrawTankAndProjectiles(deltaTimeSeconds, tank1);

	// Update tank2
	DrawTankAndProjectiles(deltaTimeSeconds, tank2);

	// Draw trajectories last to see in background
	glLineWidth(1.5f);
	RenderMesh2D(tank1->getTrajectoryMesh(), shaders["VertexColor"], glm::mat3(1.0f));
	RenderMesh2D(tank2->getTrajectoryMesh(), shaders["VertexColor"], glm::mat3(1.0f));
	glLineWidth(1);
}


void TankWars::FrameEnd()
{
}


void TankWars::OnInputUpdate(float deltaTime, int mods)
{
    const float speed = 90;
	const float angularSpeed = RADIANS(60);

	// Move tank1
    if (window->KeyHold(GLFW_KEY_A)) {
		tank1->moveTank(-speed * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        tank1->moveTank(speed * deltaTime);
    }
	// Rotate tank1 gun
    if (window->KeyHold(GLFW_KEY_W)) {
		tank1->rotateGun(angularSpeed * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_S)) {
		tank1->rotateGun(-angularSpeed * deltaTime);
    }

	// Move tank2
	if (window->KeyHold(GLFW_KEY_LEFT)) {
		tank2->moveTank(-speed * deltaTime);
	}
	if (window->KeyHold(GLFW_KEY_RIGHT)) {
		tank2->moveTank(speed * deltaTime);
	}
	// Rotate tank2 gun
	if (window->KeyHold(GLFW_KEY_UP)) {
		tank2->rotateGun(angularSpeed * deltaTime);
	}
	if (window->KeyHold(GLFW_KEY_DOWN)) {
		tank2->rotateGun(-angularSpeed * deltaTime);
	}
}


void TankWars::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_SPACE) {
		tank1->shoot();
	}
	if (key == GLFW_KEY_ENTER) {
		tank2->shoot();
	}
}


void TankWars::OnKeyRelease(int key, int mods)
{
}


void TankWars::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}


void TankWars::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}


void TankWars::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void TankWars::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void TankWars::OnWindowResize(int width, int height)
{
}
