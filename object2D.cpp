#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string& name,
    glm::vec3 topLeftCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = topLeftCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, -length, 0), glm::vec3(0.0f, 0.0f, 0.0f)),
        VertexFormat(corner + glm::vec3(0, -length, 0), glm::vec3(0.0f, 0.0f, 0.0f))
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {

        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateTankPart(
    const std::string& name,
    const std::vector<VertexFormat>& vertices,
    const std::vector<unsigned int>& indices)
{
    Mesh* tankPart = new Mesh(name);
    tankPart->InitFromData(vertices, indices);
    return tankPart;
}