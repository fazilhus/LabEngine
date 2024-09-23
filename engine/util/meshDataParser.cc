#include "config.h"
#include "meshDataParser.h"

#include <fstream>
#include <iostream>
#include <cstdio>
#include <unordered_map>

namespace Utils {
    
    void MeshDataParser::ParseOBJ(
		const std::string& path,
		std::vector<Resource::VertexData>& vdata,
		std::vector<unsigned int>& idata) {
        if (path.substr(path.size() - 4, 4) != ".obj") {
			std::cerr << "[ERROR] wrong file type\n";
			return;
		}

		std::ifstream in{ path, std::ios::in | std::ios::binary };
		if (!in.is_open()) {
			std::cerr << "[ERROR] could not open .obj file: " << path << '\n';
			return;
		}

		OBJMeshData data{};

        std::vector<std::string> fbuff;
		std::vector<std::string> vbuff;

		std::string line;

		std::vector<std::streamoff> pos_lines;
		std::vector<std::streamoff> norm_lines;
		std::vector<std::streamoff> uv_lines;

		while (std::getline(in, line)) {
			if (FirstToken(line) == "o") break;
		}
		auto data_pos = in.tellg();

		while (!in.eof()) {
			auto fpos = in.tellg();
			std::getline(in, line);
			auto token = FirstToken(line);
			if (token == "v") {
				pos_lines.push_back(fpos);
			}
			if (token == "vn") {
				norm_lines.push_back(fpos);
			}
			if (token == "vt") {
				uv_lines.push_back(fpos);
			}
			if (token == "s") break;
		}

		std::size_t vertex_count = 0;
		std::unordered_map<std::string, unsigned int> map;

		while (std::getline(in, line)) {
			auto token = FirstToken(line);
			if (token == "f") {
				fbuff.clear();
				Split(Tail(line), fbuff);
				if (fbuff.size() != 3) {
					std::cerr << "[ERROR] corrupted OBJ file: face should have 3 values\n";
					return;
				}
				
				unsigned int posi[3], normi[3], uvi[3];
				for (std::size_t i = 0; i < fbuff.size(); ++i) {
					if (map.find(fbuff[i]) == map.end()) {
						auto res = std::sscanf(fbuff[i].c_str(), "%u/%u/%u", &posi[i], &uvi[i], &normi[i]);
						posi[i]--;
						uvi[i]--;
						normi[i]--;

						auto curr_pos = in.tellg();

						vdata.push_back({});
						in.seekg(pos_lines[posi[i]], in.beg);
						std::getline(in, line);
						Split(Tail(line), vbuff);
						for (std::size_t j = 0; j < 3; ++j) {
							vdata.back().pos[j] = std::stof(vbuff[j]);
						}

						in.seekg(uv_lines[uvi[i]], in.beg);
						std::getline(in, line);
						Split(Tail(line), vbuff);
						for (std::size_t j = 0; j < 2; ++j) {
							vdata.back().uv[j] = std::stof(vbuff[j]);
						}

						in.seekg(norm_lines[normi[i]], in.beg);
						std::getline(in, line);
						Split(Tail(line), vbuff);
						for (std::size_t j = 0; j < 3; ++j) {
							vdata.back().norm[j] = std::stof(vbuff[j]);
						}
						map[fbuff[i]] = vertex_count;
						idata.push_back(vertex_count);
						vertex_count++;

						in.seekg(curr_pos, in.beg);
					}
					else {
						idata.push_back(map[fbuff[i]]);
					}
				}
			}
		}

		in.close();
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