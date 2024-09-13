#include "config.h"
#include "meshDataParser.h"

#include <fstream>
#include <iostream>

namespace Utils {
    
    OBJMeshData MeshDataParser::ParseOBJ(const std::string& path) {
        if (path.substr(path.size() - 4, 4) != ".obj") {
			std::cerr << "[ERROR] wrong file type\n";
			return OBJMeshData{};
		}

		std::ifstream in{ path };
		if (!in.is_open()) {
			std::cerr << "[ERROR] could not open .obj file: " << path << '\n';
			return OBJMeshData{};
		}

		OBJMeshData data{};

        std::vector<std::string> buff;

		std::string line;
		while (std::getline(in, line)) {
			auto token = FirstToken(line);
			if (token == "#" || token == "o" || token == "s") {
				continue;
			}

			if (token == "v") {
				buff.clear();
				Split(Tail(line), buff);
				if (buff.size() != 3) {
					std::cerr << "[ERROR] corrupted OBJ file: vertex should have 2 values\n";
					return OBJMeshData{};
				}
				data.positions.push_back({});
				data.positions.back()[0] = std::stof(buff[0]);
				data.positions.back()[1] = std::stof(buff[1]);
				data.positions.back()[2] = std::stof(buff[2]);
			}

			if (token == "vt") {
				buff.clear();
				Split(Tail(line), buff);
				if (buff.size() != 2) {
					std::cerr << "[ERROR] corrupted OBJ file: uv should have 2 values\n";
					return OBJMeshData{};
				}
				data.uvs.push_back({});
				data.uvs.back()[0] = std::stof(buff[0]);
				data.uvs.back()[1] = std::stof(buff[1]);
			}

			if (token == "f") {
				buff.clear();
				Split(Tail(line), buff);
				if (buff.size() != 3) {
					std::cerr << "[ERROR] corrupted OBJ file: face should have 3 values\n";
					return OBJMeshData{};
				}
				
				for (std::size_t i = 0; i < 3; ++i) {
					std::size_t delimeter = buff[i].find_first_of('/');
					if (delimeter == std::string::npos) {
						std::cerr << "[ERROR] corrupted OBJ file\n";
						return OBJMeshData{};
					}
					auto spos_idx = buff[i].substr(0, delimeter);
					auto suv_idx = buff[i].substr(delimeter + 1);
					data.pos_idx.push_back(std::stoi(spos_idx) - 1);
					data.uv_idx.push_back(std::stoi(suv_idx) - 1);
				}
			}
		}

        in.close();
        return data;
    }

    std::string MeshDataParser::FirstToken(const std::string& line) {
        if (line.empty()) return "";

		std::size_t start = line.find_first_not_of(" \t");
		std::size_t end = line.find_first_of(" \t", start);
		if (start != std::string::npos && end != std::string::npos) {
			return line.substr(start, end - start);
		}
		else if (start != std::string::npos) {
			return line.substr(start);
		}

		return "";
    }

    std::string MeshDataParser::Tail(const std::string& line) {
        size_t token_start = line.find_first_not_of(" \t");
		size_t space_start = line.find_first_of(" \t", token_start);
		size_t tail_start = line.find_first_not_of(" \t", space_start);
		size_t tail_end = line.find_last_not_of(" \t");
		if (tail_start != std::string::npos && tail_end != std::string::npos) {
			return line.substr(tail_start, tail_end - tail_start + 1);
		} else if (tail_start != std::string::npos) {
			return line.substr(tail_start);
		}
		return "";
    }

	void MeshDataParser::Split(const std::string& line, std::vector<std::string>& out) {
        out.clear();
		std::string temp{};
		for (char c : line) {
			if (std::isspace(c)) {
				out.push_back(temp);
				temp.clear();
			} else {
				temp.push_back(c);
			}
		}
		if (!temp.empty()) out.push_back(temp);
    }

} // Utils