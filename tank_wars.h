#pragma once

#include "components/simple_scene.h"


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
         const glm::vec4 bgColor = { 0.251f, 0.722f, 1, 1 };

         const glm::vec3 terrainColor = { 0.3f, 0.55f, 0.03f };
         const int terrainOffset = 30;
         const int terrainStep = 10;
         std::vector<VertexFormat> terrainVertices;
         std::vector<float *> heightMap;

         glm::vec2 sc{ 20, 20 };
    };
}   // namespace m1
