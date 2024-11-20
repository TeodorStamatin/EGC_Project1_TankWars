#pragma once

#include <string>
#include <vector>
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
#include "utils/gl_utils.h"

namespace object2D
{
    Mesh* CreateSquare(const std::string& name, glm::vec3 topLeftCorner, float length, glm::vec3 color, bool fill);

    Mesh* CreateTankPart(const std::string& name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
}
