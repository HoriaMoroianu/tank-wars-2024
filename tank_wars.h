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
		 static TankWars* GetInstance() {
			 if (instance == nullptr) {
				 instance = new TankWars();
			 }
			 return instance;
		 }
        ~TankWars();

        void Init() override;
        bool checkTankCollision(glm::vec2 projectilePos, float projectileSize);

     private:
        TankWars();
        void CreateTerrain();
        void DrawTankAndProjectiles(const float deltaTime, Tank* tank);
        void InitTank(Tank** tank, glm::vec2 pos, glm::vec3 primaryColor, glm::vec3 secondaryColor);

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

    public:
         static std::vector<float *> heightMap;
		 static glm::vec2 screenSize;

     private:
		 static TankWars* instance;

         class tw::Tank *tank1;
         class tw::Tank *tank2;

         std::vector<VertexFormat> terrainVertices;
         std::vector<unsigned int> indices;

         const int terrainOffset = 30;
    };
}   // namespace m1
