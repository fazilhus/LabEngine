#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2022 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "render/camera.h"
#include "render/light.h"
#include "render/node.h"


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
		void HandleInput();

	private:
		std::shared_ptr<Resource::Shader> shader;
		std::shared_ptr<Resource::Shader> lsrcShader;
		Render::LightManager lm;
		Resource::ShaderManager sm;
		Resource::GraphicsNode obj1;
		Resource::GraphicsNode obj2;
		//Resource::GraphicsNode obj3;
		Render::Camera* camera;
		Display::Window* window;
		float time;
		float prev_time;
		float dt;
	};

} // namespace Example