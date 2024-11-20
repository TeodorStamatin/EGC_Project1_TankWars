#pragma once

#include "components/simple_scene.h"
#include <glm/glm.hpp>
#include <vector>
#include <random>

#define MIN_TURRET_ANGLE (-M_PI / 2)
#define MAX_TURRET_ANGLE (M_PI / 2)
#define TERRAIN_WIDTH 1281
#define CANNON_SPEED 400
#define GRAVITATIONAL_ACCELERATION 300

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

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

    protected:

        std::vector<float> vector1;
        std::vector<float> vector2;
        std::vector<double> heights;
        void GenerateHeights(std::vector<float>& amplitudes, std::vector<float>& frequencies);
        float f(std::vector<float>& amplitudes, std::vector<float>& frequencies, double t);
        void trajectoryLine(float deltaTimeSeconds, float cannonX, float cannonY, float angle, float ta);
        void Tema1::renderClouds(float cloudScale);

        float timer = 0.0f;
        bool isVisible = true;
        float maxTimer = 2.0f;

        glm::mat3 modelMatrix;
		float heartScale = 2.0f;
        float cloudScale = 5.0f;
        bool cloudIncDec = 1;
        bool heartIncDec = 1;
		bool landslide = false;
		bool gameOver = false;


        struct Tank {
            float posX;
            float posY;
            int health;
            float dirX;
            float dirY;
            float angle;
            float turretAngle;

            Tank(float x, float y, int hp, float dirX, float dirY, float angle, float turretAngle) : posX(x), posY(y), health(hp), dirX(dirX), dirY(dirY), angle(angle), turretAngle(turretAngle) {}
        };

        Tank Tank1;
        Tank Tank2;

        struct Cannon {
            float x, y;
            float vx, vy;
            bool active;
            int tank;

            Cannon(int tank, float startX, float startY, float unghi, float viteza) : tank(tank), x(startX), y(startY), active(true) {
                vx = viteza * cos(unghi);
                vy = viteza * sin(unghi);
            }

			void updatePos(float deltaTimeSeconds, std::vector<double>& heights) {
                if (!active) 
                    return;
                x += vx * deltaTimeSeconds;
                y += vy * deltaTimeSeconds;
                vy -= GRAVITATIONAL_ACCELERATION * deltaTimeSeconds;

                if (x <= 0 || x >= TERRAIN_WIDTH || y < 0) {
                    active = false;
                    return;
                }

				if (y <= heights[x]) {
                    active = false;
                    float radius = 40;
                    for (int i = std::max(0, static_cast<int>(x) - 40); i <= std::min(static_cast<int>(x) + 40, static_cast<int>(heights.size()) - 1); i++) {
                        float dist = sqrt(radius * radius - (x - i) * (x - i));
                        float margin = y - dist;

                        if (heights[i] > margin) {
                            heights[i] = margin;
                            if (heights[i] < 5)
                                heights[i] = 5;
                        }
                    }
                }
            }
        };

        std::vector<Cannon> cannons;
    };
}   // namespace m1
