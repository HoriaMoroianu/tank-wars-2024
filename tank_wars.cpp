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

Mesh* CreateTrapeze(const std::string name, const glm::vec3 color)
{
    std::vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(-1, -0.25f, 0), color),
        VertexFormat(glm::vec3(1, -0.25f, 0), color),
        VertexFormat(glm::vec3(0.75f, 0.25f, 0), color),
        VertexFormat(glm::vec3(-0.75f, 0.25f, 0), color)
    };
    std::vector<unsigned int> indices
    {
        0, 1, 2,
        2, 3, 0
    };

    Mesh* trapeze = new Mesh(name);
    trapeze->SetDrawMode(GL_TRIANGLES);
    trapeze->InitFromData(vertices, indices);
    return trapeze;
}

Mesh* CreateCircle(const std::string name, const glm::vec3 color, const int subdivisions = 15)
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

    Mesh* circle = new Mesh(name);
    circle->SetDrawMode(GL_TRIANGLE_FAN);
    circle->InitFromData(vertices, indices);
    return circle;
}

Mesh* CreateSquare(const std::string name, const glm::vec3 color)
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

    Mesh* square = new Mesh(name);
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

    CreateTerrain();

    // TODO: for tank: body -> head -> base -> gun
    AddMeshToList(CreateTrapeze("tank_base", glm::vec3(0.29f, 0.251f, 0.184f)));
    AddMeshToList(CreateTrapeze("tank_body", glm::vec3(0.388f, 0.337f, 0.243f)));
    AddMeshToList(CreateCircle("tank_gun", glm::vec3(0.388f, 0.337f, 0.243f), 31));
    AddMeshToList(CreateSquare("tank_nose", glm::vec3(0, 0, 0)));


    AddMeshToList(CreateCircle("origin", glm::vec3(1, 0.5f, 0)));
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
    //RenderMesh2D(meshes["terrain"], shaders["VertexColor"], glm::mat3(1));

    glm::mat3 originMatrix = transform2D::Translate(window->GetResolution().x / 2, window->GetResolution().y / 2);
    //glm::mat3 originMatrix = transform2D::Translate(300 + sc.x, 200 + sc.y);
    float tScale = 150.f;

    RenderMesh2D(meshes["origin"], shaders["VertexColor"], originMatrix * transform2D::Scale(3.5f));

    RenderMesh2D(meshes["origin"], shaders["VertexColor"], originMatrix 
        * transform2D::Translate(0, 0.5f * tScale) * transform2D::Scale(3.5f));

    glm::mat3 tankScale = transform2D::Scale(tScale);
    glm::mat3 modelMatrix{};

    // Tank Body
    modelMatrix = glm::mat3(1.0f);
    modelMatrix *= originMatrix;
    modelMatrix *= tankScale;
    modelMatrix *= transform2D::Translate(0, 0.33f);
    modelMatrix *= transform2D::Scale(1.25f, 0.7f);
    RenderMesh2D(meshes["tank_body"], shaders["VertexColor"], modelMatrix);

    // Tank Base
    modelMatrix = glm::mat3(1.0f);
    modelMatrix *= originMatrix;
    modelMatrix *= tankScale;
    modelMatrix *= transform2D::Translate(0, 0.125f);
    modelMatrix *= transform2D::Scale(1, 0.5f);
    modelMatrix *= transform2D::Rotate(M_PI);
    RenderMesh2D(meshes["tank_base"], shaders["VertexColor"], modelMatrix);

    // Tank Head
    modelMatrix = glm::mat3(1.0f);
    modelMatrix *= originMatrix;
    modelMatrix *= tankScale;
    modelMatrix *= transform2D::Translate(0, 0.5f);
    modelMatrix *= transform2D::Scale(0.5f);
    RenderMesh2D(meshes["tank_gun"], shaders["VertexColor"], modelMatrix);

    // Tank Nose
    modelMatrix = glm::mat3(1.0f);
    modelMatrix *= originMatrix;
    modelMatrix *= tankScale;
    modelMatrix *= transform2D::Translate(0, 0.5f);
    modelMatrix *= transform2D::Rotate(-M_PI / angle);
    modelMatrix *= transform2D::Translate(0, 0.5f);
    modelMatrix *= transform2D::Scale(0.12f, 0.85f);
    modelMatrix *= transform2D::Translate(0, 0.5f);
    RenderMesh2D(meshes["tank_nose"], shaders["VertexColor"], modelMatrix);
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
