#include "config.h"
#include "node.h"

namespace Resource {

	GraphicsNode::GraphicsNode() {
	}

	GraphicsNode::GraphicsNode(const std::string& meshPath, const std::string& texPath,
		const std::string& vsPath, const std::string& fsPath) {
		texture = std::make_shared<Texture>(texPath);

		MeshBuilder mb{ meshPath };
		mesh = std::make_shared<Mesh>(mb.CreateMesh(texture));

		shader = std::make_shared<Shader>(Shader(vsPath, fsPath));
	}

	GraphicsNode::GraphicsNode(const GraphicsNode& other)
		: texture(other.texture), mesh(other.mesh), shader(other.shader) {
	}

	GraphicsNode::~GraphicsNode() {
	}

	void GraphicsNode::DeInit() {
		mesh->DeInit();
		texture->Unload();
		shader->Cleanup();
	}

	void GraphicsNode::SetMesh(std::shared_ptr<Mesh> meshPtr) {
		mesh = meshPtr;
	}

	void GraphicsNode::SetTexture(std::shared_ptr<Texture> texPtr) {
		texture = texPtr;
	}

	void GraphicsNode::SetShader(std::shared_ptr<Shader> shaderPtr) {
		shader = shaderPtr;
	}

	void GraphicsNode::Draw(const Render::Camera& cam) const {
		shader->Use();
		shader->SetLight();

		shader->UploadUniformMat4fv("transform", transform);
		shader->UploadUniformMat4fv("view", cam.GetView());
		shader->UploadUniformMat4fv("perspective", cam.GetPerspective());
		shader->UploadUniform3fv("cam_pos", cam.GetCameraPos());

		mesh->Draw();
	}

} // Resource