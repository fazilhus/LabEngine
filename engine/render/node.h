#pragma once

#include "render/mesh.h"
#include "render/shader.h"
#include "math/mat4.h"
#include "render/camera.h"

#include <memory>

namespace Resource {

	class GraphicsNode {
	public:
		Math::mat4 transform;

	private:
		std::shared_ptr<Mesh> mesh;
		std::vector<std::shared_ptr<Material>> materials;

	public:
		GraphicsNode() = default;
		GraphicsNode(const GraphicsNode& other);
		~GraphicsNode();

		void DeInit();

		void SetMesh(const std::shared_ptr<Mesh>& meshPtr) { mesh = meshPtr; }
		void PushMaterial(const std::shared_ptr<Material>& mat);

		const Mesh& GetMesh() const { return *mesh; }
		const std::vector<std::shared_ptr<Material>>& GetMaterials() const { return materials; }

		Mesh& GetMesh() { return *mesh; }
		std::vector<std::shared_ptr<Material>>& GetMaterials() { return materials; }

		void Draw(const Render::Camera& cam) const;
	};

	// TODO create a node manager

} // Resource