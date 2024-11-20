#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>
#include <random>
#include <cmath>
#include <GLFW/glfw3.h>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"
#include "utils/gl_utils.h"

using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
    : Tank1(1100.f, 0.f, 4, -1, 0, 0, 0),
    Tank2(110.f, 0.f, 4, 1, 0, 0, 0)
{
}

Tema1::~Tema1()
{
}


void Tema1::Init()
{

    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 1;

    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(0.94f, 0.82f, 0.12f), true);
    AddMeshToList(square1);

    //tank1
    {
        vector<VertexFormat> vertices = {
            VertexFormat(glm::vec3(-5, 0,  0), glm::vec3(0.86f, 0.08f, 0.24f)),
            VertexFormat(glm::vec3(5, 0,  0), glm::vec3(0.86f, 0.08f, 0.24f)),
            VertexFormat(glm::vec3(-10,  5,  0), glm::vec3(0.86f, 0.08f, 0.24f)),
            VertexFormat(glm::vec3(10,  5,  0), glm::vec3(0.86f, 0.08f, 0.24f)),
            VertexFormat(glm::vec3(-15, 5,  0), glm::vec3(0.73f, 0.04f, 0.12f)),
            VertexFormat(glm::vec3(15, 5,  0), glm::vec3(0.73f, 0.04f, 0.12f)),
            VertexFormat(glm::vec3(-10, 10, 0), glm::vec3(0.6f, 0.0f, 0.0f)),
            VertexFormat(glm::vec3(10, 10, 0), glm::vec3(0.6f, 0.0f, 0.0f))

        };

        int numCircleSegments = 30;
        float radius = 5.0f;
        for (int i = 0; i <= numCircleSegments; i++) {
            float angle = M_PI * i / numCircleSegments;
            float x = radius * cos(angle);
            float y = radius * sin(angle) + 10;
            vertices.push_back(VertexFormat(glm::vec3(x, y, 0), glm::vec3(0.6f, 0.0f, 0.0f)));
        }

        vector<unsigned int> indices = {
            0, 1, 2, 1, 2, 3,
            4, 5, 6, 5, 6, 7
        };

        for (int i = 0; i < numCircleSegments; i++) {
            indices.push_back(8);
            indices.push_back(9 + i);
            indices.push_back(9 + (i + 1));
        }

        Mesh* combinedTank = object2D::CreateTankPart("combinedTank1", vertices, indices);
        AddMeshToList(combinedTank);
    }

    //tank 2
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-5, 0,  0), glm::vec3(0.12f, 0.56f, 1.0f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(5, 0,  0), glm::vec3(0.12f, 0.56f, 1.0f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-10,  5,  0), glm::vec3(0.12f, 0.56f, 1.0f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(10,  5,  0), glm::vec3(0.12f, 0.56f, 1.0f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-15, 5,  0), glm::vec3(0.06f, 0.27f, 0.75f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(15, 5,  0), glm::vec3(0.06f, 0.27f, 0.75f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-10,  10,  0), glm::vec3(0.0f, 0.0f, 0.55f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(10,  10,  0), glm::vec3(0.0f, 0.0f, 0.55f), glm::vec3(0.2, 0.8, 0.6)),

        };

        int numCircleSegments = 30;
        float radius = 5.0f;
        for (int i = 0; i <= numCircleSegments; i++) {
            float angle = M_PI * i / numCircleSegments;
            float x = radius * cos(angle);
            float y = radius * sin(angle) + 10;
            vertices.push_back(VertexFormat(glm::vec3(x, y, 0), glm::vec3(0.0f, 0.0f, 0.55f)));
        }

        vector<unsigned int> indices = {
            0, 1, 2, 1, 2, 3,
            4, 5, 6, 5, 6, 7
        };

        for (int i = 0; i < numCircleSegments; i++) {
            indices.push_back(8);
            indices.push_back(9 + i);
            indices.push_back(9 + (i + 1));
        }

        Mesh* combinedTank2 = object2D::CreateTankPart("combinedTank2", vertices, indices);
        AddMeshToList(combinedTank2);
    }

    //turret
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-3, 0,  0), glm::vec3(0.0f, 0.0f, 0.0f)),
            VertexFormat(glm::vec3(3, 0,  0), glm::vec3(0.0f, 0.0f, 0.0f)),
            VertexFormat(glm::vec3(-3,  25,  0), glm::vec3(0.0f, 0.0f, 0.0f)),
            VertexFormat(glm::vec3(3,  25,  0), glm::vec3(0.0f, 0.0f, 0.0f)),
        };

        vector<unsigned int> indices = {
            0, 1, 2, 1, 2, 3
        };

        Mesh* combinedTurret = object2D::CreateTankPart("combinedTurret", vertices, indices);
        AddMeshToList(combinedTurret);
    }

    // cannon
    {
        vector<VertexFormat> vertices;
        vector<unsigned int> indices;

        int numCircleSegments = 30;
        float radius = 5.0f; 
        vertices.push_back(VertexFormat(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.55f)));
        for (int i = 0; i <= numCircleSegments; i++) {
            float angle = 2 * M_PI * i / numCircleSegments;
            float x = radius * cos(angle);
            float y = radius * sin(angle) + 10;
            vertices.push_back(VertexFormat(glm::vec3(x, y, 0), glm::vec3(0.0f, 0.0f, 0.0f)));
        }

        for (int i = 0; i < numCircleSegments; i++) {
            indices.push_back(0);
            indices.push_back(i + 1);
            indices.push_back(i + 2);
        }

        Mesh* cannon = object2D::CreateTankPart("cannon", vertices, indices);
        AddMeshToList(cannon);
    }

    //heart
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(0, 0.0f,  0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-6.6f, 10,  0), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(6.6f,  10,  0), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.2, 0.8, 0.6)),

        };
        int numCircleSegments = 30;
        float radius = 3.7f;
        for (int i = 0; i <= numCircleSegments; i++) {
            float angle = 2 * M_PI * i / numCircleSegments;
            float x = radius * cos(angle) - 3;
            float y = radius * sin(angle) + 11;
            vertices.push_back(VertexFormat(glm::vec3(x, y, 0), glm::vec3(1.0f, 0.0f, 0.0f)));
        }

        for (int i = 0; i <= numCircleSegments; i++) {
            float angle = 2 * M_PI * i / numCircleSegments;
            float x = radius * cos(angle) + 3;
            float y = radius * sin(angle) + 11;
            vertices.push_back(VertexFormat(glm::vec3(x, y, 0), glm::vec3(1.0f, 0.0f, 0.0f)));
        }

		vector<unsigned int> indices = {
			0, 1, 2
		};

        for (int i = 0; i < numCircleSegments; i++) {
            indices.push_back(3);
            indices.push_back(4 + i);
            indices.push_back(4 + (i + 1));
        }

		for (int i = 0; i < numCircleSegments; i++) {
			indices.push_back(33);
			indices.push_back(34 + i);
			indices.push_back(34 + (i + 1));
		}

		Mesh* heart = object2D::CreateTankPart("heart", vertices, indices);
		AddMeshToList(heart);
    }

    //healthbar
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-41, 0,  0), glm::vec3(1,1,1)),
            VertexFormat(glm::vec3(41, 0,  0), glm::vec3(1,1,1)),
            VertexFormat(glm::vec3(-41,  22,  0), glm::vec3(1,1,1)),
            VertexFormat(glm::vec3(41,  22,  0), glm::vec3(1,1,1)),
        };

        vector<unsigned int> indices = {
            0, 1, 3, 2, 0
        };

        glPointSize(5);
        glLineWidth(3);
        Mesh* healthbar = new Mesh("healthbar");
        healthbar->InitFromData(vertices, indices);
        healthbar->SetDrawMode(GL_LINE_STRIP);
        AddMeshToList(healthbar);
    }

    //bar for healthbar
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(0, 0,  0), glm::vec3(1,1,1)),
            VertexFormat(glm::vec3(20, 0,  0), glm::vec3(1,1,1)),
            VertexFormat(glm::vec3(0,  20,  0), glm::vec3(1,1,1)),
            VertexFormat(glm::vec3(20,  20,  0), glm::vec3(1,1,1)),
        };

        vector<unsigned int> indices = {
            0, 1, 2, 1, 2, 3
        };

        Mesh* bar = object2D::CreateTankPart("bar", vertices, indices);
        AddMeshToList(bar);
    }

    //cloud
    {
        vector<VertexFormat> vertices;
        vector<unsigned int> indices;

        int numCircleSegments = 30;
        float radius = 5.0f;

        float xOffset = 10.0f;
        float yOffset = 10.0f;
        vertices.push_back(VertexFormat(glm::vec3(xOffset, yOffset, 0.0f), glm::vec3(0.90f, 0.90f, 0.90f)));
        for (int i = 0; i <= numCircleSegments; i++) {
            float angle = 2 * M_PI * i / numCircleSegments;
            float x = radius * cos(angle) + xOffset;
            float y = radius * sin(angle) + yOffset;
            vertices.push_back(VertexFormat(glm::vec3(x, y, 0), glm::vec3(1.0f, 1.0f, 1.0f)));
        }

        for (int i = 0; i < numCircleSegments; i++) {
            indices.push_back(0);
            indices.push_back(i + 1);
            indices.push_back(i + 2);
        }

        float xOffset2 = 5.0f;
        float yOffset2 = 15.0f;
        int centerIndex2 = vertices.size();
        vertices.push_back(VertexFormat(glm::vec3(xOffset2, yOffset2, 0.0f), glm::vec3(0.90f, 0.90f, 0.90f)));
        for (int i = 0; i <= numCircleSegments; i++) {
            float angle = 2 * M_PI * i / numCircleSegments;
            float x = radius * cos(angle) + xOffset2;
            float y = radius * sin(angle) + yOffset2;
            vertices.push_back(VertexFormat(glm::vec3(x, y, 0), glm::vec3(1.0f, 1.0f, 1.0f)));
        }

        for (int i = 0; i < numCircleSegments; i++) {
            indices.push_back(centerIndex2);
            indices.push_back(centerIndex2 + i + 1);
            indices.push_back(centerIndex2 + i + 2);
        }

        float xOffset3 = 13.0f;
        float yOffset3 = 13.0f;

        int centerIndex3 = vertices.size();
        vertices.push_back(VertexFormat(glm::vec3(xOffset3, yOffset3, 0.0f), glm::vec3(0.90f, 0.90f, 0.90f)));

        for (int i = 0; i <= numCircleSegments; i++) {
            float angle = 2 * M_PI * i / numCircleSegments;
            float x = radius * cos(angle) + xOffset3;
            float y = radius * sin(angle) + yOffset3;
            vertices.push_back(VertexFormat(glm::vec3(x, y, 0), glm::vec3(1.0f, 1.0f, 1.0f)));
        }

        for (int i = 0; i < numCircleSegments; i++) {
            indices.push_back(centerIndex3);
            indices.push_back(centerIndex3 + i + 1);
            indices.push_back(centerIndex3 + i + 2);
        }

        float xOffset4 = 1.5f;
        float yOffset4 = 12.0f;

        int centerIndex4 = vertices.size();
        vertices.push_back(VertexFormat(glm::vec3(xOffset4, yOffset4, 0.0f), glm::vec3(0.90f, 0.90f, 0.90f)));

        for (int i = 0; i <= numCircleSegments; i++) {
            float angle = 2 * M_PI * i / numCircleSegments;
            float x = radius * cos(angle) + xOffset4;
            float y = radius * sin(angle) + yOffset4;
            vertices.push_back(VertexFormat(glm::vec3(x, y, 0), glm::vec3(1.0f, 1.0f, 1.0f)));
        }

        for (int i = 0; i < numCircleSegments; i++) {
            indices.push_back(centerIndex4);
            indices.push_back(centerIndex4 + i + 1);
            indices.push_back(centerIndex4 + i + 2);
        }

        Mesh* cloud = object2D::CreateTankPart("cloud", vertices, indices);
        AddMeshToList(cloud);
    }

	//X1 wins game over
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(20, 50,  0), glm::vec3(0.86f, 0.08f, 0.24f)),
			VertexFormat(glm::vec3(25, 55,  0), glm::vec3(0.86f, 0.08f, 0.24f)),
			VertexFormat(glm::vec3(30, 25,  0), glm::vec3(0.86f, 0.08f, 0.24f)),
			VertexFormat(glm::vec3(32.5f, 37.5f,  0), glm::vec3(0.86f, 0.08f, 0.24f)),
            VertexFormat(glm::vec3(80, 75,  0), glm::vec3(0.86f, 0.08f, 0.24f)),
            VertexFormat(glm::vec3(75, 80,  0), glm::vec3(0.86f, 0.08f, 0.24f)),
		};

		vector<unsigned int> indices = {
			0, 1, 2, 3, 4, 5
		};

        glPointSize(5);
        glLineWidth(3);
		Mesh* x1 = new Mesh("x1");
		x1->InitFromData(vertices, indices);
		x1->SetDrawMode(GL_TRIANGLE_STRIP);
		AddMeshToList(x1);
	}
    //X2 wins game over
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(20, 50,  0), glm::vec3(0.0f, 0.0f, 1.0f)),
            VertexFormat(glm::vec3(25, 55,  0), glm::vec3(0.0f, 0.0f, 1.0f)),
            VertexFormat(glm::vec3(30, 25,  0), glm::vec3(0.0f, 0.0f, 1.0f)),
            VertexFormat(glm::vec3(32.5f, 37.5f,  0), glm::vec3(0.0f, 0.0f, 1.0f)),
            VertexFormat(glm::vec3(80, 75,  0), glm::vec3(0.0f, 0.0f, 1.0f)),
            VertexFormat(glm::vec3(75, 80,  0), glm::vec3(0.0f, 0.0f, 1.0f)),
        };

        vector<unsigned int> indices = {
            0, 1, 2, 3, 4, 5
        };

        glPointSize(5);
        glLineWidth(3);
        Mesh* x2 = new Mesh("x2");
        x2->InitFromData(vertices, indices);
        x2->SetDrawMode(GL_TRIANGLE_STRIP);
        AddMeshToList(x2);
    }


    vector1 = { 1, 1.5f, 0.4f };
    vector2 = { 1, 0.5f, 3 };
    GenerateHeights(vector1, vector2);

    Tank1.dirY = heights[Tank1.posX];
    Tank2.dirY = heights[Tank2.posX];
}

float Tema1::f(std::vector<float>& A, std::vector<float>& omega, double t) {
    float result = 0.0;

    for (int i = 0; i < A.size(); i++) {
        result += A[i] * sin(omega[i] * t);
    }

    return result;
}

void Tema1::GenerateHeights(std::vector<float>& A, std::vector<float>& omega) {
    heights.resize(TERRAIN_WIDTH + 1);

    for (int x = 0; x < TERRAIN_WIDTH + 1; x++) {
        heights[x] = (3 + f(A, omega, static_cast<double>(x) / 100.0)) * 100;
    }
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.6f, 0.8f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::trajectoryLine(float deltaTimeSeconds, float cannonX, float cannonY, float angle, float ta) {
    std::vector<VertexFormat> vert;
    std::vector<unsigned int> ind;

    float vx, vy, x, y;

    x = cannonX;
    y = cannonY;

    vx = 400.0f * cos(angle);
    vy = 400.0f * sin(angle);

    float time = 0;
    float timeStep = 0.01f;

    for (int i = 0; i < 1000 || (x + vx * time <= TERRAIN_WIDTH && x + vx * time >= 0); i++) {
        vert.push_back(VertexFormat(glm::vec3(x, y, 0), glm::vec3(0.5f, 0.5f, 0.5f)));
        ind.push_back(i);
        x += vx * time;
        y += vy * time;
        vy -= 320.0f * time;
        time += timeStep;
    }


    if (vert.empty()) {
        return;
    }

    glPointSize(5);
    glLineWidth(3);
    Mesh* trajectory = new Mesh("trajectory");
    trajectory->InitFromData(vert, ind);
    trajectory->SetDrawMode(GL_LINE_STRIP);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(0, 15 * sin(ta));
    modelMatrix *= transform2D::Scale(1, 1);
    RenderMesh2D(trajectory, shaders["VertexColor"], modelMatrix);

}

void Tema1::renderClouds(float cloudScale) {
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(70, 600);
    modelMatrix *= transform2D::Scale(cloudScale - 2, cloudScale - 2);
    RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(300, 570);
    modelMatrix *= transform2D::Scale(cloudScale, cloudScale);
    RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(500, 490);
    modelMatrix *= transform2D::Scale(cloudScale + 2, cloudScale + 2);
    RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(600, 490);
    modelMatrix *= transform2D::Scale(cloudScale + 2, cloudScale + 2);
    RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(800, 460);
    modelMatrix *= transform2D::Scale(cloudScale - 0.5f, cloudScale - 0.5f);
    RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(1000, 400);
    modelMatrix *= transform2D::Scale(cloudScale, cloudScale);
    RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(1150, 550);
    modelMatrix *= transform2D::Scale(cloudScale + 1, cloudScale + 1);
    RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);
}

void Tema1::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    if (gameOver) {
        cannons.clear();
    }

    timer += deltaTimeSeconds;
    if (timer >= maxTimer) {
        timer = 0.0f;
        isVisible = !isVisible;
    }
    if (gameOver) {
        if (Tank1.health == 0 && isVisible) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(300, -50);
            modelMatrix *= transform2D::Scale(8, 8);
            RenderMesh2D(meshes["x2"], shaders["VertexColor"], modelMatrix);
        }
         else if (Tank2.health == 0 && isVisible) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= transform2D::Translate(300, -50);
			modelMatrix *= transform2D::Scale(8, 8);
			RenderMesh2D(meshes["x1"], shaders["VertexColor"], modelMatrix);
        }
    }

	// RENDER HEARTS
	for (int i = 0; i < Tank2.health; i++) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= transform2D::Translate(30 + i * 50, 660);
		modelMatrix *= transform2D::Scale(heartScale, heartScale);
		RenderMesh2D(meshes["heart"], shaders["VertexColor"], modelMatrix);
	}
    for (int i = 0; i < Tank1.health; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(1250 - i * 50, 660);
        modelMatrix *= transform2D::Scale(heartScale, heartScale);
        RenderMesh2D(meshes["heart"], shaders["VertexColor"], modelMatrix);
    }

    // HEALTH SCALE ANIMATION
    if (heartIncDec) {
        heartScale += 1.0f * deltaTimeSeconds;
    }
    if (!heartIncDec) {
        heartScale -= 1.0f * deltaTimeSeconds;
    }
    if (heartScale >= 3) {
        heartIncDec = !heartIncDec;
    }
    if (heartScale <= 2) {
        heartIncDec = !heartIncDec;
    }

    // CLOUD SCALE ANIMATION
    if (cloudIncDec) {
        cloudScale += 0.7f * deltaTimeSeconds;
    }
    if (!cloudIncDec) {
        cloudScale -= 0.7f * deltaTimeSeconds;
    }
    if (cloudScale >= 7) {
        cloudIncDec = !cloudIncDec;
    }
    if (cloudScale <= 5) {
        cloudIncDec = !cloudIncDec;
    }

    // RENDER CLOUDS
	renderClouds(cloudScale);

    //RENDER TANK1
    if (Tank1.health > 0) {

        Tank1.posY = heights[Tank1.posX];
        double angle = 0;
        if (Tank1.dirX == 1) {
            Tank1.dirY = heights[Tank1.posX + 1] - heights[Tank1.posX];
            angle = atan2(Tank1.dirY, -1);
        }
        else {
            Tank1.dirY = heights[Tank1.posX] - heights[Tank1.posX - 1];
            angle = atan2(Tank1.dirY, 1);
        }
        Tank1.angle = angle;

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(Tank1.posX, Tank1.posY);
        modelMatrix *= transform2D::Rotate(Tank1.angle);
        modelMatrix *= transform2D::Scale(2, 2);
        RenderMesh2D(meshes["combinedTank1"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(Tank1.posX, Tank1.posY);
        modelMatrix *= transform2D::Rotate(Tank1.angle);
        modelMatrix *= transform2D::Translate(0, 25);
        modelMatrix *= transform2D::Rotate(Tank1.turretAngle);
        modelMatrix *= transform2D::Scale(1, 1);
        RenderMesh2D(meshes["combinedTurret"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(Tank1.posX, Tank1.posY);
        modelMatrix *= transform2D::Rotate(Tank1.angle);
        modelMatrix *= transform2D::Translate(0, 60);
        modelMatrix *= transform2D::Scale(1, 1);
        RenderMesh2D(meshes["healthbar"], shaders["VertexColor"], modelMatrix);

		for (int i = 0; i < Tank1.health; i++) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= transform2D::Translate(Tank1.posX, Tank1.posY);
            modelMatrix *= transform2D::Rotate(Tank1.angle);
            modelMatrix *= transform2D::Translate(- 40 + i * 20, 61);
			modelMatrix *= transform2D::Scale(1, 1);
			RenderMesh2D(meshes["bar"], shaders["VertexColor"], modelMatrix);
		}
    }

	//RENDER TANK2
    if (Tank2.health > 0) {
        Tank2.posY = heights[Tank2.posX];
        double angle = 0;
        if (Tank2.dirX == 1) {
            Tank2.dirY = heights[Tank2.posX + 1] - heights[Tank2.posX];
            angle = atan2(Tank2.dirY, 1);
        }
        else {
            Tank2.dirY = heights[Tank2.posX] - heights[Tank2.posX - 1];
            angle = atan2(Tank2.dirY, -1);
        }
        Tank2.angle = angle;

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(Tank2.posX, Tank2.posY);
        modelMatrix *= transform2D::Rotate(Tank2.angle);
        modelMatrix *= transform2D::Scale(2, 2);
        RenderMesh2D(meshes["combinedTank2"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(Tank2.posX, Tank2.posY);
        modelMatrix *= transform2D::Rotate(Tank2.angle);
        modelMatrix *= transform2D::Translate(0, 25);
        modelMatrix *= transform2D::Rotate(Tank2.turretAngle);
        modelMatrix *= transform2D::Scale(1, 1);
        RenderMesh2D(meshes["combinedTurret"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(Tank2.posX, Tank2.posY);
        modelMatrix *= transform2D::Rotate(Tank2.angle);
        modelMatrix *= transform2D::Translate(0, 60);
        modelMatrix *= transform2D::Scale(1, 1);
        RenderMesh2D(meshes["healthbar"], shaders["VertexColor"], modelMatrix);

        for (int i = 0; i < Tank2.health; i++) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(Tank2.posX, Tank2.posY);
            modelMatrix *= transform2D::Rotate(Tank2.angle);
            modelMatrix *= transform2D::Translate(-40 + i * 20, 61);
            modelMatrix *= transform2D::Scale(1, 1);
            RenderMesh2D(meshes["bar"], shaders["VertexColor"], modelMatrix);
        }
    }

	//RENDER TERRAIN
    for (int i = 0; i < TERRAIN_WIDTH + 1; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(i, heights[i]);
        if (i < TERRAIN_WIDTH) {
            modelMatrix *= transform2D::ShearY(static_cast<float>(heights[i + 1] - heights[i]));
        }
        if (i < TERRAIN_WIDTH) {
            modelMatrix *= transform2D::Scale(1, heights[i] + (static_cast<float>(heights[i + 1] + heights[i])));
        }
        else {
            modelMatrix *= transform2D::Scale(1, heights[i]);
        }
        RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
    }

	//RENDER TRAJECTORY
    if (Tank1.health > 0) {
        float startX = Tank1.posX + 25 * cos(Tank1.angle + (M_PI / 2));
        float startY = Tank1.posY + 6 * sin(Tank1.angle + (M_PI / 2));
        trajectoryLine(deltaTimeSeconds, startX, startY, Tank1.turretAngle + Tank1.angle + (M_PI / 2), Tank1.angle + (M_PI / 2));
    }
	if (Tank2.health > 0) {
        float startX = Tank2.posX + 25 * cos(Tank2.angle + (M_PI / 2));
        float startY = Tank2.posY + 6 * sin(Tank2.angle + (M_PI / 2));
        trajectoryLine(deltaTimeSeconds, startX, startY, Tank2.turretAngle + Tank2.angle + (M_PI / 2), Tank2.angle + (M_PI / 2));
	}

	//RENDER CANNONS
    for (auto& cannon : cannons) {
        if (cannon.active) {
            float distX, distY;

            if (cannon.tank == 1) {
                distX = Tank2.posX - cannon.x;
                distY = Tank2.posY - cannon.y;
                float dist = sqrt(distX * distX + distY * distY);
                if (dist < 35) {
                    Tank2.health--;
                    cannon.active = false;
					if (Tank2.health == 0)
						gameOver = true;
                    break;
                }
            }
            if (cannon.tank == 2) {
                distX = Tank1.posX - cannon.x;
                distY = Tank1.posY - cannon.y;
                float dist = sqrt(distX * distX + distY * distY);
                if (dist < 35) {
                    Tank1.health--;
                    cannon.active = false;
					if (Tank1.health == 0)
						gameOver = true;
                    break;
                }
            }

            cannon.updatePos(deltaTimeSeconds, heights);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(cannon.x, cannon.y);
            modelMatrix *= transform2D::Scale(1, 1);
            RenderMesh2D(meshes["cannon"], shaders["VertexColor"], modelMatrix);
        }
    }
    
    //REMOVE CANNONS
    cannons.erase(std::remove_if(cannons.begin(), cannons.end(), [](const Cannon& c) {
        return !c.active;
        }), cannons.end());

    //LANDSLIDE
    float maxDiff = 5.0f;
    float transferRate = 5.0f;
	int speed = 50;

    for (int i = 0; i < TERRAIN_WIDTH && landslide; i++) {
        float heightDiff = heights[i] - heights[i + 1];

        if (std::abs(heightDiff) > maxDiff) {
            float terrainTransfer = transferRate * deltaTimeSeconds * speed;

            if (heightDiff > 0) {
                heights[i] -= terrainTransfer;
                heights[i + 1] += terrainTransfer;
            }
            else {
                heights[i] += terrainTransfer;
                heights[i + 1] -= terrainTransfer;
            }

            if (heights[i] < 0)
                heights[i] = 0;
            if (heights[i + 1] < 0)
                heights[i + 1] = 0;
        }
    }

}

void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (!gameOver) {
        if (window->KeyHold(GLFW_KEY_D)) {
            if (Tank2.posX < TERRAIN_WIDTH - 1)
                Tank2.posX += 100 * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_A)) {
            if (Tank2.posX > 0)
                Tank2.posX -= 100 * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_RIGHT)) {
            if (Tank1.posX < TERRAIN_WIDTH - 1)
                Tank1.posX += 100 * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_LEFT)) {
            if (Tank1.posX >= 1)
                Tank1.posX -= 100 * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_UP) && Tank1.turretAngle < MAX_TURRET_ANGLE) {
            Tank1.turretAngle += 2.0f * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_DOWN) && Tank1.turretAngle > MIN_TURRET_ANGLE) {
            Tank1.turretAngle -= 2.0f * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_W) && Tank2.turretAngle < MAX_TURRET_ANGLE) {
            Tank2.turretAngle += 2.0f * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_S) && Tank2.turretAngle > MIN_TURRET_ANGLE) {
            Tank2.turretAngle -= 2.0f * deltaTime;
        }
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
    if (!gameOver) {
        if (key == GLFW_KEY_ENTER && Tank1.health > 0) {
            float startX = Tank1.posX + 25 * cos(Tank1.angle + (M_PI / 2));
            float startY = Tank1.posY + 5 + 6 * sin(Tank1.angle + (M_PI / 2));
            cannons.emplace_back(1, startX, startY, Tank1.turretAngle + Tank1.angle + (M_PI / 2), CANNON_SPEED);
        }
        if (key == GLFW_KEY_SPACE && Tank2.health > 0) {
            float startX = Tank2.posX + 25 * cos(Tank2.angle + (M_PI / 2));
            float startY = Tank2.posY + 5 + 6 * sin(Tank2.angle + (M_PI / 2));
            cannons.emplace_back(2, startX, startY, Tank2.turretAngle + Tank2.angle + (M_PI / 2), CANNON_SPEED);
        }
        if (key == GLFW_KEY_L) {
            landslide = !landslide;
        }
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
