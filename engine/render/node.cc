#include "config.h"
#include "node.h"

#include <iostream>

namespace Resource {

	GraphicsNode::GraphicsNode(const GraphicsNode& other)
		: mesh(other.mesh) {
	}

	GraphicsNode::~GraphicsNode() {
	}

	void GraphicsNode::DeInit() {
		mesh->DeInit();
	}

	void GraphicsNode::PushMaterial(const std::shared_ptr<Material>& mat) {
		auto i = materials.size();

		if (mesh->GetGroupsSize() <= i) {
			std::cerr << "[ERROR] number of materials is greater than number of primitives\n";
			return;
		}

		materials.push_back(mat);
		mesh->GetGroups()[i].mat = materials.back();
	}

	void GraphicsNode::Draw(const Render::Camera& cam) const {
		//shader.lock()->Use();
		//
		//shader.lock()->UploadUniformMat4fv("transform", transform);
		//shader.lock()->UploadUniformMat4fv("view", cam.GetView());
		//shader.lock()->UploadUniformMat4fv("perspective", cam.GetPerspective());
		//shader.lock()->UploadUniform3fv("cam_pos", cam.GetCameraPos());
		//
		//mesh->Draw();
		auto& groups = mesh->GetGroups();
		for (std::size_t i = 0; i < mesh->GetGroupsSize(); ++i) {
			auto shader = groups[i].mat.lock()->GetShader().lock();

			shader->Use();
			shader->UploadUniformMat4fv("transform", transform);
			shader->UploadUniformMat4fv("view", cam.GetView());
			shader->UploadUniformMat4fv("perspective", cam.GetPerspective());
			shader->UploadUniform3fv("cam_pos", cam.GetCameraPos());

			mesh->DrawGroup(i);
			shader->UnUse();
		}
	}

} // Resource