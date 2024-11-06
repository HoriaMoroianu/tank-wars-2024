#include "lab_m1/tank-wars-2024/tank_wars.h"

using namespace tw;
using namespace m1;


TankWars::TankWars()
{
}


TankWars::~TankWars()
{
}


void TankWars::CreateTerrain()
{
    // Terrain height generation function 
    auto generator = [](int x) -> float { return 100.f * std::sin(0.01f * x) + 300.f; };

    for (int i = -terrainOffset; i < window->GetResolution().x + terrainOffset; i += terrainStep) {
        terrainVertices.push_back(VertexFormat(glm::vec3(i, generator(i), 0), terrainColor));
        terrainVertices.push_back(VertexFormat(glm::vec3(i, -terrainOffset, 0), terrainColor));
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

    CreateTerrain();
	tank1 = Tank();

    //AddMeshToList(CreateCircle("origin", glm::vec3(1, 0.5f, 0)));
}


void TankWars::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void TankWars::Update(float deltaTimeSeconds)
{
    //RenderMesh2D(meshes["terrain"], shaders["VertexColor"], glm::mat3(1.0f));

    //glm::mat3 originMatrix = transform2D::Translate(window->GetResolution().x / 2, window->GetResolution().y / 2);

    /*RenderMesh2D(meshes["origin"], shaders["VertexColor"], originMatrix * transform2D::Scale(3.5f));

    RenderMesh2D(meshes["origin"], shaders["VertexColor"], originMatrix 
        * transform2D::Translate(0, 0.5f * tScale) * transform2D::Scale(3.5f));*/

	tank1.Update();
}


void TankWars::FrameEnd()
{
}


void TankWars::OnInputUpdate(float deltaTime, int mods)
{
    float speed = 50;
    if (window->KeyHold(GLFW_KEY_W)) {
        sc.y += speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        sc.y -= speed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        sc.x += speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        sc.x -= speed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_Q)) {
        angle += 1.5f * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_E)) {
        angle -= 1.5f * deltaTime;
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
