#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2022 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "render/mesh.h"
#include "render/shader.h"
#include "render/grid.h"
#include "render/camera.h"
#include "math/mat4.h"
#include "render/node.h"

#include <memory>

namespace Example {

	class ImGuiExampleApp : public Core::App {
	public:
		/// constructor
		ImGuiExampleApp();
		/// destructor
		~ImGuiExampleApp();

		/// open app
		bool Open();
		/// clone app
		void Close();
		/// run app
		void Run();

	private:
		std::shared_ptr<Resource::Shader> shader;
		//Resource::Mesh mesh;
		Resource::GraphicsNode cube;
		Render::Grid* grid;
		Render::Camera* camera;
		Display::Window* window;
	};

} // namespace Example