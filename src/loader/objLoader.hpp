#pragma once

#include "glimac/common.hpp"
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <vector>

std::vector<glimac::ShapeVertex> loadOBJ(const std::string &filename);
