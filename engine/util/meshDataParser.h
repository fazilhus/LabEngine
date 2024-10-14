#pragma once

#include <string>
#include <vector>

#include "render/mesh.h"

namespace Utils {

    class MeshDataParser {
    public:
        void ParseOBJ(
            const std::string& path,
            Resource::VertexData& vdata,
            std::vector<unsigned int>& idata);

    private:
        std::string FirstToken(const std::string& line);
        std::string Tail(const std::string& line);
        void Split(const std::string& line, std::vector<std::string>& out);
    };

} // Utils