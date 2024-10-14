#pragma once

#include "gl/glew.h"

#include <vector>
#include <string>

#include "math/vec2.h"
#include "math/vec3.h"
#include "material.h"

namespace Resource {

	struct PrimitiveGroup {
		std::size_t indices;
		std::size_t offset;
		std::weak_ptr<Material> mat;
	};

	struct VertexData;

	class Mesh {
		GLuint vao;
		GLuint vbos[3];
		GLuint ebo;

		std::vector<PrimitiveGroup> groups;

	public:
		Mesh();
		Mesh(const Mesh& other);
		~Mesh() = default;

		Mesh& operator=(const Mesh& other);

		void Init(const VertexData& vb, const std::vector<GLuint>& ib, const std::size_t* sizes,
			const std::size_t* offsets, std::size_t count);
		void DeInit();

		void Draw() const;
		void DrawGroup(std::size_t i) const;

		void PushPrimitive(PrimitiveGroup group);

		std::size_t GetGroupsSize() const { return groups.size(); }
		const std::vector<PrimitiveGroup>& GetGroups() const { return groups; }
		std::vector<PrimitiveGroup>& GetGroups() { return groups; }


	private:
		void Bind() const;
		void UnBind() const;
	};

	struct VertexData {
		std::vector<Math::vec3> pos;
		std::vector<Math::vec3> norm;
		std::vector<Math::vec2> uv;

		void clear();
		void reserve(std::size_t a, std::size_t b, std::size_t c);
		void push_back(Math::vec3 p, Math::vec3 n, Math::vec2 u);
	};

	class MeshBuilder {
	protected:
		VertexData vertexes;
		std::vector<GLuint> indices;

	public:
		MeshBuilder() = default;
		MeshBuilder(const MeshBuilder&) = delete;
		MeshBuilder(MeshBuilder&&) = delete;
		virtual ~MeshBuilder() {}

		MeshBuilder& operator=(const MeshBuilder&) = delete;
		MeshBuilder& operator=(MeshBuilder&&) = delete;

		virtual void ReadMeshData(const std::filesystem::path& path) = 0;
		virtual Mesh CreateMesh() const = 0;
	};

	class OBJMeshBuilder final : public MeshBuilder {
	public:
		explicit OBJMeshBuilder(const std::filesystem::path& path);
		~OBJMeshBuilder() override = default;

		void ReadMeshData(const std::filesystem::path& path) override;
		Mesh CreateMesh() const override;
	};

	class GLTFMeshBuilder final : public MeshBuilder {
		explicit GLTFMeshBuilder(const std::filesystem::path& path);
		~GLTFMeshBuilder() override = default;

		void ReadMeshData(const std::filesystem::path& path) override;
		Mesh CreateMesh() const override;
	};

} // Resource