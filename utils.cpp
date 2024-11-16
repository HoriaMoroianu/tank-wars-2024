#include "lab_m1/tank-wars-2024/utils.h"
#include "lab_m1/tank-wars-2024/tank_wars.h"

#include <iostream>

float tw::TerrainGenerator(const int x)
{
	// TODO: randomize mountain position
	float mountain = 250.f * pow(M_E, -pow(x - 200, 2) / 20000.f);
    return 50 * sin(M_PI / 600 * x) +
        25 * sin(M_PI / 300 * x)
        + 10 * sin(M_PI / 150 * x)
        + mountain + 250.f;
}

void tw::printTerrain()
{
	std::vector<float*> &heightMap = TankWars::heightMap;
    float min = INFINITY;
    float max = -INFINITY;
    float sum = 0;
    for (int i = 1; i < heightMap.size(); i++) {
        float val1 = *heightMap[i - 1];
        float val2 = *heightMap[i];

        if (abs(val1 - val2) < min) {
            min = abs(val1 - val2);
        }
        if (abs(val1 - val2) > max) {
            max = abs(val1 - val2);
        }
        std::cout << "Height difference: " << abs(val1 - val2) << std::endl;
        sum += abs(val1 - val2);
    }
    std::cout << "Height difference: ";
    std::cout << "Min: " << min << " Max: " << max << std::endl;
    std::cout << "Average: " << sum / heightMap.size() << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}

void tw::aux(const int index1, const int index2, const float deltaTime)
{
	if (index1 < 0 || index1 >= TankWars::heightMap.size() || 
        index2 < 0 || index2 >= TankWars::heightMap.size()) {
		return;
	}
	float* height1 = TankWars::heightMap[index1];
	float* height2 = TankWars::heightMap[index2];

	float diff = *height1 - *height2;

    if (abs(diff) <= terrainThreshold) {
		return;
	}

    float sign = diff > 0 ? 1 : -1;
	*height1 -= sign * terrainDiffEpsilon * deltaTime;
	*height2 += sign * terrainDiffEpsilon * deltaTime;
}

void tw::flattenTerrain(const float deltaTime)
{
	std::vector<float*> &heights = TankWars::heightMap;

	std::vector<int> ind;
    for (int i = 0; i < heights.size(); i++) {
		ind.push_back(i);
    }

    std::sort(ind.begin(), ind.end(), [heights](int a, int b) { return *heights[a] > *heights[b]; });

    for (int i : ind) {
        aux(i, i + 1, deltaTime);
        aux(i, i - 1, deltaTime);
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

Mesh* tw::CreateSquare(const std::string name, const glm::vec3 color)
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
