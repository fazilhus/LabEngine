#pragma once

#include <string>
#include <vector>

#include "math/vec2.h"
#include "math/vec3.h"
#include "render/mesh.h"

namespace Utils {

    struct OBJMeshData {
        std::vector<Math::vec3> positions;
        std::vector<Math::vec3> normals;
        std::vector<Math::vec2> uvs;
        std::vector<unsigned int> pos_idx;
        std::vector<unsigned int> norm_idx;
        std::vector<unsigned int> uv_idx;
    };

    class MeshDataParser {
    public:
        void ParseOBJ(
            const std::string& path,
            std::vector<Resource::VertexData>& vdata,
            std::vector<unsigned int>& idata);

    private:
        std::string FirstToken(const std::string& line);
        std::string Tail(const std::string& line);
        void Split(const std::string& line, std::vector<std::string>& out);
    };

} // Utils