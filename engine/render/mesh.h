#pragma once

#include "gl/glew.h"

#include <vector>
#include <string>
#include <fstream>

#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "texture.h"

namespace Resource {

	struct PrimitiveGroup {
		std::size_t indices;
		std::size_t offset;
		Texture tex;
	};

	class Mesh {
	private:
		GLuint vao;
		GLuint vbo;
		GLuint ebo;

		std::vector<PrimitiveGroup> groups;

	public:
		Mesh();
		Mesh(const Mesh& other);
		~Mesh() = default;

		Mesh& operator=(const Mesh& other);

		void Init(GLfloat* vb, GLuint* ib, const std::size_t* sizes, const std::size_t* offsets, 
			const std::size_t verticies, const std::size_t triangles, const std::size_t count);
		void DeInit();

		void Draw(const GLuint programHandle) const;
		void Draw(std::size_t i) const;

		void PushPrimitive(PrimitiveGroup group);

		static Mesh CreateQuadMesh(const float width = 1.0f, const float height = 1.0f);
		static Mesh CreateCubeMesh(const float width = 1.0f, const float height = 1.0f, const float depth = 1.0f);

	private:
		void Bind() const;
		void UnBind() const;
	};

	struct VertexData {
		Math::vec3 pos;
		//Math::vec3 norm;
		Math::vec2 uv;
	};

	class MeshBuilder {
	private:
		std::vector<VertexData> vertexes;
		std::vector<GLuint> indices;

	public:
		MeshBuilder(const std::string& path);
		~MeshBuilder() = default;

		void ReadMeshData(const std::string& path);
		Mesh CreateMesh() const;

	private:
		std::string FirstToken(const std::string& line) const;
		std::string Tail(const std::string& line) const;
		void Split(const std::string& line, std::vector<std::string>& out) const;
		void ParseOBJ(std::ifstream& in, std::vector<Math::vec3>& positions, std::vector<Math::vec2>& uvs,
			std::vector<unsigned int>& pos_idx, std::vector<unsigned int>& uv_idx);
	};

} // Resource