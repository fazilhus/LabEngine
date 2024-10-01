#include "config.h"
#include "meshDataParser.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <unordered_map>

namespace Utils {

	enum class Face {
		triangle = 0,
		quad,
	};
    
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

		std::stringstream contents;
    	contents << in.rdbuf();

		OBJMeshData data{};

        std::vector<std::string> fbuff;
		std::vector<std::string> vbuff;

		std::string line;

		std::vector<std::streamoff> pos_lines;
		std::vector<std::streamoff> norm_lines;
		std::vector<std::streamoff> uv_lines;

		/*while (std::getline(in, line)) {
			if (FirstToken(line) == "o") break;
		}
		auto data_pos = in.tellg();*/

		while (!contents.eof()) {
			auto fpos = contents.tellg();
			std::getline(contents, line);
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
		vdata.reserve(pos_lines.size() + norm_lines.size() + uv_lines.size());
		idata.reserve(pos_lines.size() + norm_lines.size() + uv_lines.size());

		Face face = Face::triangle;
		auto pos = contents.tellg();
		std::getline(contents, line);
		fbuff.clear();
		Split(Tail(line), fbuff);
		if (fbuff.size() == 3) {
			face = Face::triangle;
		} else if (fbuff.size() == 4) {
			face = Face::quad;
		} else {
			std::cerr << "[ERROR] corrupted OBJ file: face should have 3 or 4 values\n";
			return;
		}

		contents.seekg(pos);
		while (std::getline(contents, line)) {
			auto token = FirstToken(line);
			if (token == "f") {
				fbuff.clear();
				Split(Tail(line), fbuff);
				if (fbuff.size() != 3 && fbuff.size() != 4) {
					std::cerr << "[ERROR] corrupted OBJ file: face should have 3 or 4 values\n";
					return;
				}
				
				unsigned int posi[4], normi[4], uvi[4];
				for (std::size_t i = 0; i < fbuff.size(); ++i) {
					if (map.find(fbuff[i]) == map.end()) {
						sscanf_s(fbuff[i].c_str(), "%u/%u/%u", &posi[i], &uvi[i], &normi[i]);
						posi[i]--;
						uvi[i]--;
						normi[i]--;

						auto curr_pos = contents.tellg();

						Math::vec3 p;
						contents.seekg(pos_lines[posi[i]], contents.beg);
						std::getline(contents, line);
						sscanf_s(line.c_str(), "%f %f %f", p[0], p[1], p[2]);

						Math::vec2 u;
						contents.seekg(uv_lines[uvi[i]], contents.beg);
						std::getline(contents, line);
						sscanf_s(line.c_str(), "%f %f", u[0], u[1]);

						Math::vec3 n;
						contents.seekg(norm_lines[normi[i]], contents.beg);
						std::getline(contents, line);
						sscanf_s(line.c_str(), "%f %f %f", n[0], n[1], n[2]);
						
						vdata.push_back({p, n, u});
						map[fbuff[i]] = vertex_count;
						vertex_count++;

						contents.seekg(curr_pos, in.beg);
					}
				}

				switch (face) {
				case Face::triangle: {
						idata.push_back(map[fbuff[0]]);
						idata.push_back(map[fbuff[1]]);
						idata.push_back(map[fbuff[2]]);
						break;
					}
				case Face::quad: {
						idata.push_back(map[fbuff[0]]);
						idata.push_back(map[fbuff[1]]);
						idata.push_back(map[fbuff[2]]);
						idata.push_back(map[fbuff[2]]);
						idata.push_back(map[fbuff[3]]);
						idata.push_back(map[fbuff[0]]);
						break;
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