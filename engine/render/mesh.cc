#include "config.h"
#include "mesh.h"

#include <cassert>
#include <iostream>

namespace Resource {

	Mesh::Mesh()
		: vao(0), vbo(0), ebo(0) {}

	Mesh::Mesh(const Mesh& other)
		: vao(other.vao), vbo(other.vbo), ebo(other.ebo), groups(other.groups) {
	}

	Mesh& Mesh::operator=(const Mesh& other) {
		this->vao = other.vao;
		this->vbo = other.vbo;
		this->ebo = other.ebo;
		this->groups = other.groups;
		return *this;
	}

	void Mesh::Init(GLfloat* vb, GLuint* ib, const std::size_t* sizes, const std::size_t* offsets,
		const std::size_t verticies, const std::size_t triangles, const std::size_t count) {
		glGenVertexArrays(1, &this->vao);
		glBindVertexArray(this->vao);

		std::size_t stride{ 0 };
		for (std::size_t i = 0; i < count; ++i) stride += sizes[i];

		glGenBuffers(1, &this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * stride * verticies, vb, GL_STATIC_DRAW);

		glGenBuffers(1, &this->ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3 * triangles, ib, GL_STATIC_DRAW);

		for (std::size_t i = 0; i < count; ++i) {
			glVertexAttribPointer(i, sizes[i], GL_FLOAT, GL_FALSE, sizeof(GLfloat) * stride, (GLvoid*)(sizeof(GLfloat) * offsets[i]));
			glEnableVertexAttribArray(i);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Mesh::DeInit() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
		for (auto el : groups) {
			el.tex.Unload();
		}
	}

	void Mesh::Draw(const GLuint programHandle) const {
		Bind();
		for (auto el : groups) {
			//GLuint loc = glGetUniformLocation(programHandle, "face_col");
			//glUniform4fv(loc, 1, &el.color[0]);
			el.tex.Bind();
			glDrawElements(GL_TRIANGLES, el.indices, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * el.offset));
			el.tex.UnBind();
		}
		UnBind();
	}

	void Mesh::Draw(std::size_t i) const {
		Bind();
		groups[i].tex.Bind();
		glDrawElements(GL_TRIANGLES, groups[i].indices, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * groups[i].offset));
		groups[i].tex.UnBind();
		UnBind();
	}

	void Mesh::PushPrimitive(PrimitiveGroup group) {
		groups.push_back(group);
	}

	void Mesh::Bind() const {
		glBindVertexArray(this->vao);
	}

	void Mesh::UnBind() const {
		glBindVertexArray(0);
	}

	Mesh Mesh::CreateQuadMesh(const float width, const float height) {
		Mesh mesh{};
		GLfloat vb[] =
		{
			-0.5f * width, 0.01f, -0.5f * height,	1.0f, 0.0f, 0.0f, 1.0f,		1.0f, 0.0f,
			-0.5f * width, 0.01f,  0.5f * height,	0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 0.0f,
			 0.5f * width, 0.01f,  0.5f * height,	0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f,
			 0.5f * width, 0.01f, -0.5f * height,	0.0f, 0.0f, 0.0f, 1.0f,		1.0f, 1.0f
		};

		GLuint ib[] = {
			0, 2, 3,
			0, 1, 2,
		};
		std::size_t sizes[] = { 3, 4, 2 };
		std::size_t offsets[] = { 0, 3, 7 };
		mesh.Init(vb, ib, sizes, offsets, 4, 2, 3);
		Texture tex{};
		tex.LoadFromFile("../projects/CameraExample/res/img.png");
		mesh.PushPrimitive({ 6, 0, tex });
		return mesh;
	}

	Mesh Mesh::CreateCubeMesh(const float width, const float height, const float depth)
	{
		Mesh mesh{};
		GLfloat vb[] =
		{
			-0.5f * width,	-0.5f * height,	-0.5f * depth,	1, 0, 0, 1,		1.0f, 0.0f,
			-0.5f * width,	 0.5f * height,	-0.5f * depth,	0, 1, 0, 1,		0.0f, 0.0f,
			0.5f * width,	 0.5f * height,	-0.5f * depth,	0, 0, 0, 1,		0.0f, 1.0f,
			0.5f * width,	-0.5f * height,	-0.5f * depth,	0, 0, 1, 1,		1.0f, 1.0f,
			-0.5f * width,	-0.5f * height,	 0.5f * depth,	1, 0, 0, 1,		1.0f, 1.0f,
			-0.5f * width,	 0.5f * height,	 0.5f * depth,	0, 1, 0, 1,		0.0f, 1.0f,
			0.5f * width,	 0.5f * height,	 0.5f * depth,	0, 0, 0, 1,		0.0f, 0.0f,
			0.5f * width,	-0.5f * height,	 0.5f * depth,	0, 0, 1, 1,		1.0f, 0.0f,
		};

		GLuint ib[] = {
			0, 2, 3,
			0, 1, 2,
			0, 5, 1,
			0, 4, 5,
			2, 6, 3,
			3, 6, 7,
			0, 7, 4,
			0, 3, 7,
			1, 6, 2,
			1, 5, 6,
			4, 6, 5,
			4, 7, 6
		};
		std::size_t sizes[] = { 3, 4, 2 };
		std::size_t offsets[] = { 0, 3, 7 };
		mesh.Init(vb, ib, sizes, offsets, 8, 12, 3);
		Texture tex{};
		tex.LoadFromFile("../projects/CameraExample/res/img.png");
		mesh.PushPrimitive({ 36, 0, tex });
		//mesh.PushPrimitive({ 6, 0,  tex });
		//mesh.PushPrimitive({ 6, 6,  tex});
		//mesh.PushPrimitive({ 6, 12, tex});
		//mesh.PushPrimitive({ 6, 18, tex});
		//mesh.PushPrimitive({ 6, 24, tex});
		//mesh.PushPrimitive({ 6, 30, tex});
		return mesh;
	}

	MeshBuilder::MeshBuilder(const std::string& path) {
		ReadMeshData(path);
	}

	void MeshBuilder::ReadMeshData(const std::string& path) {
		if (path.substr(path.size() - 4, 4) != ".obj") {
			std::cerr << "[ERROR] wrong file type\n";
			return;
		}

		std::ifstream in{ path };
		if (!in.is_open()) {
			std::cerr << "[ERROR] could not open .obj file: " << path << '\n';
			return;
		}

		std::vector<Math::vec3> positions;
		std::vector<Math::vec2> uvs;
		std::vector<unsigned int> pos_idx;
		std::vector<unsigned int> uv_idx;

		ParseOBJ(in, positions, uvs, pos_idx, uv_idx);
		in.close();

		for (std::size_t i = 0; i <= pos_idx.size() - 3; i += 3) {
			vertexes.push_back(VertexData{positions[pos_idx[i]],     uvs[uv_idx[i]]});
			vertexes.push_back(VertexData{positions[pos_idx[i + 1]], uvs[uv_idx[i + 1]]});
			vertexes.push_back(VertexData{positions[pos_idx[i + 2]], uvs[uv_idx[i + 2]]});
			indices.push_back(i);indices.push_back(i + 1);indices.push_back(i + 2);
		}
	}

	Mesh MeshBuilder::CreateMesh() const {
		Mesh mesh{};
		std::size_t sizes[] = {3, 2};
		std::size_t offsets[] = {0, 3};
		mesh.Init((GLfloat*)vertexes.data(), (GLuint*)indices.data(), sizes, offsets, vertexes.size(), indices.size() / 3, 2);
		Texture tex{};
		tex.LoadFromFile("../projects/CameraExample/res/img.png");
		mesh.PushPrimitive({ indices.size(), 0, tex });
		return mesh;
	}

	std::string MeshBuilder::FirstToken(const std::string& line) const {
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

	std::string MeshBuilder::Tail(const std::string& line) const {
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

	void MeshBuilder::Split(const std::string& line, std::vector<std::string>& out) const {
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

	void MeshBuilder::ParseOBJ(std::ifstream& in, std::vector<Math::vec3>& positions, std::vector<Math::vec2>& uvs, 
	std::vector<unsigned int>& pos_idx, std::vector<unsigned int>& uv_idx) {
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
					return;
				}
				positions.push_back({});
				positions.back()[0] = std::stof(buff[0]);
				positions.back()[1] = std::stof(buff[1]);
				positions.back()[2] = std::stof(buff[2]);
			}

			if (token == "vt") {
				buff.clear();
				Split(Tail(line), buff);
				if (buff.size() != 2) {
					std::cerr << "[ERROR] corrupted OBJ file: uv should have 2 values\n";
					return;
				}
				uvs.push_back({});
				uvs.back()[0] = std::stof(buff[0]);
				uvs.back()[1] = std::stof(buff[1]);
			}

			if (token == "f") {
				buff.clear();
				Split(Tail(line), buff);
				if (buff.size() != 3) {
					std::cerr << "[ERROR] corrupted OBJ file: face should have 3 values\n";
					return;
				}
				
				for (std::size_t i = 0; i < 3; ++i) {
					std::size_t delimeter = buff[i].find_first_of('/');
					if (delimeter == std::string::npos) {
						std::cerr << "[ERROR] corrupted OBJ file\n";
						return;
					}
					auto spos_idx = buff[i].substr(0, delimeter);
					auto suv_idx = buff[i].substr(delimeter + 1);
					//std::cout << "string pos idx " << spos_idx << " string uv idx " << suv_idx << '\n';
					pos_idx.push_back(std::stoi(spos_idx) - 1);
					uv_idx.push_back(std::stoi(suv_idx) - 1);
					//std::cout << "pos idx " << std::stoi(spos_idx) - 1 << " uv idx " << std::stoi(suv_idx) - 1 << '\n';
				}
			}
		}
	}

} // Resource