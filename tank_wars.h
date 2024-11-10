#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tank-wars-2024/tank.h"
#include "lab_m1/tank-wars-2024/utils.h"
#include "lab_m1/tank-wars-2024/transform2D.h"


namespace tw
{
	class Tank;

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

    public:
         static std::vector<float *> heightMap;
         static const int terrainStep = 5;

     private:
         class tw::Tank tank1;
         class tw::Tank tank2;
         std::vector<VertexFormat> terrainVertices;

         const int terrainOffset = 30;

		 // TODO: remove this
		 bool toggleTerrain = true;
    };
}   // namespace m1
