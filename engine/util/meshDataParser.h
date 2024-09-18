#pragma once

#include <string>
#include <vector>
#include "math/vec2.h"
#include "math/vec3.h"

namespace Utils {

    struct OBJMeshData {
        std::vector<Math::vec3> positions;
        std::vector<Math::vec2> uvs;
        std::vector<unsigned int> pos_idx;
        std::vector<unsigned int> uv_idx;
    };

    class MeshDataParser {
    public:
        OBJMeshData ParseOBJ(const std::string& path);

    private:
        std::string FirstToken(const std::string& line);
        std::string Tail(const std::string& line);
        void Split(const std::string& line, std::vector<std::string>& out);
    };

} // Utils