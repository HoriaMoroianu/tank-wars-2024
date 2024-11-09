#include "lab_m1/tank-wars-2024/tank_wars.h"
#include <iostream>

using namespace tw;

std::vector<float*> TankWars::heightMap;

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

    std::vector<unsigned int> indices(terrainVertices.size());
    heightMap.reserve(terrainVertices.size() / 2);

    for (int i = 0; i < terrainVertices.size(); i++) {
        if ((i & 1) == 0) {
            heightMap.push_back(&terrainVertices[i].position.y);
        }
        indices[i] = i;
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

    
	for (auto &mesh : tank1.getTankMeshes()) {
		AddMeshToList(mesh);
	}
    CreateTerrain();
	tank1.moveTank(0);
}


void TankWars::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorBlueSky.r, colorBlueSky.g, colorBlueSky.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void TankWars::Update(float deltaTimeSeconds)
{
    RenderMesh2D(meshes["terrain"], shaders["VertexColor"], glm::mat3(1.0f));

	auto& tankParts = tank1.getTankParts();
	for (auto& part : tankParts) {
		RenderMesh2D(meshes[part.first], shaders["VertexColor"], part.second);
	}
}


void TankWars::FrameEnd()
{
}


void TankWars::OnInputUpdate(float deltaTime, int mods)
{
    const float speed = 90;
    if (window->KeyHold(GLFW_KEY_A)) {
		tank1.moveTank(-speed * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        tank1.moveTank(speed * deltaTime);
    }
    
	const float angularSpeed = RADIANS(60);
    if (window->KeyHold(GLFW_KEY_W)) {
		tank1.rotateGun(angularSpeed * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_S)) {
		tank1.rotateGun(-angularSpeed * deltaTime);
    }
}


void TankWars::OnKeyPress(int key, int mods)
{
    // Add key press event
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
