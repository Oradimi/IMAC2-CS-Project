#pragma once

#include "shape_vertex.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

std::vector<ShapeVertex> loadOBJ(const std::string &filename);
void addVertices(std::vector<ShapeVertex> &vertices,
                 const std::vector<glm::vec3> &positions,
                 const std::vector<glm::vec3> &normals,
                 const std::vector<glm::vec2> &texCoords,
                 const std::vector<int> &vIndices,
                 const std::vector<int> &vtIndices,
                 const std::vector<int> &vnIndices);