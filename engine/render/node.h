#pragma once

#include "render/mesh.h"
#include "render/texture.h"
#include "render/material.h"
#include "render/shader.h"
#include "math/mat4.h"
#include "render/camera.h"

#include <memory>
#include <string>

namespace Resource {

	class GraphicsNode {
	public:
		Math::mat4 transform;

	private:
		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Material> material;
		std::shared_ptr<Shader> shader;

	public:
		GraphicsNode();
		GraphicsNode(const std::string& meshPath,
			const std::string& vsPath, const std::string& fsPath);
		GraphicsNode(const GraphicsNode& other);
		~GraphicsNode();

		void DeInit();

		void SetMesh(std::shared_ptr<Mesh> meshPtr);
		void SetMaterial(std::shared_ptr<Material> matPtr);
		void SetShader(std::shared_ptr<Shader> shaderPtr);

		inline const Mesh& GetMesh() const { return *mesh.get(); };
		inline const Material& GetMaterial() const { return *material.get(); };
		inline const Shader& GetShader() const { return *shader.get(); };

		inline Mesh& GetMesh() { return *mesh.get(); };
		inline Material& GetMaterial() { return *material.get(); };
		inline Shader& GetShader() { return *shader.get(); };

		void Draw(const Render::Camera& cam) const;
	};

} // Resource