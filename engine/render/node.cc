#include "config.h"
#include "node.h"

namespace Resource {

	GraphicsNode::GraphicsNode(const GraphicsNode& other)
		: mesh(other.mesh), material(other.material), shader(other.shader) {
	}

	GraphicsNode::~GraphicsNode() {
	}

	void GraphicsNode::DeInit() {
		mesh->DeInit();
		
		shader->Cleanup();
	}

	void GraphicsNode::SetMesh(std::shared_ptr<Mesh> meshPtr) {
		mesh = meshPtr;
	}

	void GraphicsNode::SetMaterial(std::shared_ptr<Material> matPtr) {
		material = matPtr;
	}

	void GraphicsNode::SetShader(std::shared_ptr<Shader> shaderPtr) {
		shader = shaderPtr;
	}

	void GraphicsNode::Draw(const Render::Camera& cam) const {
		material->Use();
		shader->UploadUniformMat4fv("transform", transform);
		shader->UploadUniformMat4fv("view", cam.GetView());
		shader->UploadUniformMat4fv("perspective", cam.GetPerspective());
		shader->UploadUniform3fv("cam_pos", cam.GetCameraPos());

		mesh->Draw();
	}

} // Resource