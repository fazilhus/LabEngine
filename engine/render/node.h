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
		std::weak_ptr<Shader> shader;

	public:
		GraphicsNode() = default;
		GraphicsNode(const GraphicsNode& other);
		~GraphicsNode();

		void DeInit();

		void SetMesh(std::shared_ptr<Mesh> meshPtr);
		void SetShader(std::weak_ptr<Shader> shaderPtr);

		const Mesh& GetMesh() const { return *mesh; }
		const Shader& GetShader() const { return *shader.lock(); }

		Mesh& GetMesh() { return *mesh; }
		Shader& GetShader() { return *shader.lock(); }

		void Draw(const Render::Camera& cam) const;
	};

	// TODO create a node manager

} // Resource