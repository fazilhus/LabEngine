#pragma once

#include "gl/glew.h"

#include <vector>
#include <string>
#include <fstream>
#include <memory>

#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "texture.h"

namespace Resource {

	struct PrimitiveGroup {
		std::size_t indices;
		std::size_t offset;
		//std::shared_ptr<Texture> tex;
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

		void Draw(const Texture& tex) const;
		void Draw(const Texture& tex, std::size_t i) const;

		void PushPrimitive(PrimitiveGroup group);

		static Mesh CreateQuadMesh(const float width = 1.0f, const float height = 1.0f);
		static Mesh CreateCubeMesh(const float width = 1.0f, const float height = 1.0f, const float depth = 1.0f);

	private:
		void Bind() const;
		void UnBind() const;
	};

	struct VertexData {
		Math::vec3 pos;
		Math::vec3 norm;
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
	};

} // Resource