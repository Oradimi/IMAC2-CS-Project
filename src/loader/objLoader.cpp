#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>

std::vector<glm::vec3> vertices;
std::vector<glm::vec3> normals;
std::vector<glm::vec2> texCoords;
std::vector<std::vector<int>> faces;

void loadOBJ(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        } else if (type == "vn") {
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        } else if (type == "vt") {
            glm::vec2 texCoord;
            iss >> texCoord.s >> texCoord.t;
            texCoords.push_back(texCoord);
        } else if (type == "f") {
            std::vector<int> face;
            int vIndex, tIndex, nIndex;
            while (iss >> vIndex >> tIndex >> nIndex) {
                // OBJ files are 1-based, convert to 0-based
                face.push_back(vIndex - 1);
            }
            faces.push_back(face);
        }
    }
    file.close();
}
