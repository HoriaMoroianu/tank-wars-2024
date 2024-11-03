#include "lab_m1/tank-wars-2024/tank_wars.h"
#include "lab_m1/tank-wars-2024/transform2D.h"

#include <iostream>
#include <vector>

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

Mesh* CreateTrapeze(const glm::vec3 color)
{
    std::vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(-1, -0.25f, 0), color),
        VertexFormat(glm::vec3(-0.5f, -0.25f, 0), color),

        VertexFormat(glm::vec3(0.5f, -0.25f, 0), color),
        VertexFormat(glm::vec3(1, -0.25f, 0), color),

        VertexFormat(glm::vec3(0.5f, 0.25f, 0), color),
        VertexFormat(glm::vec3(-0.5f, 0.25f, 0), color)
    };
    std::vector<unsigned int> indices
    {
        0, 1, 5,
        1, 2, 4,
        2, 3, 4,
        4, 5, 1
    };

    Mesh* trapeze = new Mesh("trapeze");
    trapeze->SetDrawMode(GL_TRIANGLES);
    trapeze->InitFromData(vertices, indices);
    return trapeze;
}

Mesh* CreateCircle(const glm::vec3 color, const int subdivisions = 15)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices.push_back(VertexFormat(glm::vec3{0, 0, 0}, color));
    indices.push_back(0);

    for (int i = 0; i < subdivisions; i++) {
        indices.push_back(i + 1);

        float angle = i * 2 * M_PI / subdivisions;
        vertices.push_back(VertexFormat(glm::vec3(cos(angle), sin(angle), 0), color));
    }
    indices.push_back(1);

    Mesh* circle = new Mesh("circle");
    circle->SetDrawMode(GL_TRIANGLE_FAN);
    circle->InitFromData(vertices, indices);
    return circle;
}

Mesh* CreateSquare(const glm::vec3 color)
{
    std::vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(-0.5f, -0.5f, 0), color),
        VertexFormat(glm::vec3(0.5f, -0.5f, 0), color),
        VertexFormat(glm::vec3(0.5f, 0.5f, 0), color),
        VertexFormat(glm::vec3(-0.5f, 0.5f, 0), color)
    };
    std::vector<unsigned int> indices
    {
        0, 1, 2,
        2, 3, 0
    };

    Mesh* square = new Mesh("square");
    square->SetDrawMode(GL_TRIANGLES);
    square->InitFromData(vertices, indices);
    return square;
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

    AddMeshToList(CreateTrapeze(glm::vec3(1, 0, 0)));
    AddMeshToList(CreateCircle(glm::vec3(1, 0, 0)));
    AddMeshToList(CreateSquare(glm::vec3(1, 0, 0)));

    CreateTerrain();
    // TODO: for tank: body -> head -> base -> gun
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
    RenderMesh2D(meshes["terrain"], shaders["VertexColor"], glm::mat3(1));
    //RenderMesh2D(meshes["circle"], shaders["VertexColor"], transform2D::Scale(30));

    glm::mat3 modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(500, 350);
    modelMatrix *= transform2D::Scale(sc.x, sc.y);
    //modelMatrix *= transform2D::Scale(10);
    RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
}


void TankWars::FrameEnd()
{
}


void TankWars::OnInputUpdate(float deltaTime, int mods)
{
    float speed = 100;
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
