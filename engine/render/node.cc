#include "config.h"
#include "node.h"

namespace Resource {

	GraphicsNode::GraphicsNode(const std::shared_ptr<Model>& model)
		: transform(Math::mat4::identity()), model(model) {
	}

	void GraphicsNode::Draw(const Render::Camera& cam) const {
		model.lock()->Draw(cam, transform);
	}

} // Resource