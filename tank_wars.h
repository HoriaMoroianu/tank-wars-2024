#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tank-wars-2024/transform2D.h"
#include "lab_m1/tank-wars-2024/utils.h"
#include "lab_m1/tank-wars-2024/tank.h"


namespace m1
{
    class TankWars : public gfxc::SimpleScene
    {
     public:
        TankWars();
        ~TankWars();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void CreateTerrain();

     private:
         class tw::Tank tank1;

         const glm::vec4 bgColor = { 0.2f, 0.2f, 0.2f, 1 };

         const glm::vec3 terrainColor = { 0.3f, 0.55f, 0.03f };
         const int terrainOffset = 30;
         const int terrainStep = 10;
         std::vector<VertexFormat> terrainVertices;
         std::vector<float *> heightMap;

         glm::vec2 sc{ 1, 1 };
         float angle = 2;
    };
}   // namespace m1
