#pragma once

#include "math/mat4.h"
#include "render/camera.h"

#include <memory>

#include "model.h"

namespace Resource {

	class GraphicsNode {
	public:
		Math::mat4 transform;

	private:
		std::weak_ptr<Model> model;

	public:
		GraphicsNode() = default;
		GraphicsNode(const std::shared_ptr<Model>& model);

		void Draw(const Render::Camera& cam) const;
	};

	// TODO create a node manager

} // Resource