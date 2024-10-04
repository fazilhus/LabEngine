#include "config.h"
#include "node.h"

namespace Resource {

	GraphicsNode::GraphicsNode(const GraphicsNode& other)
		: mesh(other.mesh), shader(other.shader) {
	}

	GraphicsNode::~GraphicsNode() {
	}

	void GraphicsNode::DeInit() {
		mesh->DeInit();
		
		shader.lock()->Cleanup();
	}

	void GraphicsNode::SetMesh(std::shared_ptr<Mesh> meshPtr) {
		mesh = meshPtr;
	}

	void GraphicsNode::SetShader(std::weak_ptr<Shader> shaderPtr) {
		shader = shaderPtr;
	}

	void GraphicsNode::Draw(const Render::Camera& cam) const {
		shader.lock()->Use();

		shader.lock()->UploadUniformMat4fv("transform", transform);
		shader.lock()->UploadUniformMat4fv("view", cam.GetView());
		shader.lock()->UploadUniformMat4fv("perspective", cam.GetPerspective());
		shader.lock()->UploadUniform3fv("cam_pos", cam.GetCameraPos());

		mesh->Draw();
	}

} // Resource