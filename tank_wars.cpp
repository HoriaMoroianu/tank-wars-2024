#include "lab_m1/tank-wars-2024/tank_wars.h"
#include <iostream>

using namespace tw;

TankWars* TankWars::instance = nullptr;
std::vector<float*> TankWars::heightMap;
glm::vec2 TankWars::screenSize = glm::vec2(1280, 720);

TankWars::TankWars()
{
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

	tank1 = new Tank(glm::vec2(resolution.x / 3, 0), colorBrown, colorDarkBrown);
	tank2 = new Tank(glm::vec2(resolution.x * 2 / 3, 0), colorMillitaryGreen, colorDarkGreen);

	// Init tank1
	for (auto &mesh : tank1->getTankMeshes()) {
		AddMeshToList(mesh);
	}
	tank1->moveTank(0);

	// Init tank2
	for (auto &mesh : tank2->getTankMeshes()) {
		AddMeshToList(mesh);
	}
	tank2->moveTank(0);

	AddMeshToList(CreateCircle("projectile", colorBlack));
}

bool TankWars::checkTankCollision(glm::vec2 projectilePos)
{
	return (tank1->hitByProjectile(projectilePos) || tank2->hitByProjectile(projectilePos));
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
	if (toggleTerrain) {
		SimulateLandslide(deltaTimeSeconds);
		meshes["terrain"]->InitFromData(terrainVertices, indices);
        RenderMesh2D(meshes["terrain"], shaders["VertexColor"], glm::mat3(1.0f));
	}

	// Update tank1
    tank1->updateProjectiles(deltaTimeSeconds);
	for (auto& projectile : tank1->getProjectiles()) {
		RenderMesh2D(meshes["projectile"], shaders["VertexColor"], projectile.getModelMatrix());
	}
	tank1->moveTank(0);
	auto& tankParts = tank1->getTankParts();
	for (auto& part : tankParts) {
		RenderMesh2D(meshes[part.first], shaders["VertexColor"], part.second);
	}

	// Update tank2
	tank2->updateProjectiles(deltaTimeSeconds);
	for (auto& projectile : tank2->getProjectiles()) {
		RenderMesh2D(meshes["projectile"], shaders["VertexColor"], projectile.getModelMatrix());
	}
	tank2->moveTank(0);
	auto& tankParts2 = tank2->getTankParts();
	for (auto& part : tankParts2) {
		RenderMesh2D(meshes[part.first], shaders["VertexColor"], part.second);
	}

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

    if (window->KeyHold(GLFW_KEY_A)) {
		tank1->moveTank(-speed * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        tank1->moveTank(speed * deltaTime);
    }
    
    if (window->KeyHold(GLFW_KEY_W)) {
		tank1->rotateGun(angularSpeed * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_S)) {
		tank1->rotateGun(-angularSpeed * deltaTime);
    }

	if (window->KeyHold(GLFW_KEY_LEFT)) {
		tank2->moveTank(-speed * deltaTime);
	}
	if (window->KeyHold(GLFW_KEY_RIGHT)) {
		tank2->moveTank(speed * deltaTime);
	}

	if (window->KeyHold(GLFW_KEY_UP)) {
		tank2->rotateGun(angularSpeed * deltaTime);
	}
	if (window->KeyHold(GLFW_KEY_DOWN)) {
		tank2->rotateGun(-angularSpeed * deltaTime);
	}
}


void TankWars::OnKeyPress(int key, int mods)
{
    // Add key press event
	if (key == GLFW_KEY_SPACE) {
		tank1->shoot();
	}

	if (key == GLFW_KEY_ENTER) {
		tank2->shoot();
	}

	if (key == GLFW_KEY_T) {
		toggleTerrain = !toggleTerrain;
	}
}


void TankWars::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void TankWars::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void TankWars::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void TankWars::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void TankWars::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void TankWars::OnWindowResize(int width, int height)
{
}
