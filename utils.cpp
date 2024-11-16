#include "lab_m1/tank-wars-2024/utils.h"
#include "lab_m1/tank-wars-2024/tank_wars.h"

float tw::TerrainGenerator(const int x)
{
	// TODO: randomize mountain position
	float mountain = 250.f * pow(M_E, -pow(x - 200, 2) / 20000.f);
    return 50 * sin(M_PI / 600 * x) +
        25 * sin(M_PI / 300 * x)
        + 10 * sin(M_PI / 150 * x)
        + mountain + 250.f;
}

void tw::SimulateLandslide(const float deltaTime)
{
	std::vector<float*> &heights = TankWars::heightMap;

	for (int i = 1; i < heights.size() - 1; i++) {
		float avg = (*heights[i - 1] + *heights[i + 1]) / 2;
		if (abs(avg - *heights[i]) < terrainThreshold) {
			continue;
		}
		*heights[i] += (avg - *heights[i]) * deltaTime;
	}
}

Mesh* tw::CreateTrapeze(const std::string name, const glm::vec3 color)
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

Mesh* tw::CreateCircle(const std::string name, const glm::vec3 color, const int subdivisions)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices.push_back(VertexFormat(glm::vec3{ 0, 0, 0 }, color));
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

Mesh* tw::CreateSquare(const std::string name, const glm::vec3 color, bool fill)
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
	if (fill) {
        square->SetDrawMode(GL_TRIANGLES);
    } else {
		square->SetDrawMode(GL_LINE_LOOP);
    }
    square->InitFromData(vertices, indices);
    return square;
}

std::pair<glm::vec2, glm::vec2> tw::LocateOnTerrain(const glm::vec2 pos)
{
    glm::vec2 A{};
    glm::vec2 B{};

    A.x = (int)pos.x / terrainStep * terrainStep;
    A.y = *TankWars::heightMap[A.x / terrainStep];

    B.x = A.x + terrainStep;
    B.y = *TankWars::heightMap[B.x / terrainStep];

    return { A, B };
}
