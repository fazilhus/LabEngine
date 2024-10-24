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
#include "render/model.h"
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
		Render::LightManager lightManager;
		Resource::ShaderManager shaderManager;
		Resource::TextureManager textureManager;
		Resource::Model cube;
		Resource::Model avocado;
		Resource::Model helmet;
		Resource::Model test;
		Resource::Model sponza;
		//Resource::GraphicsNode obj1;
		//Resource::GraphicsNode obj2;
		//Resource::GraphicsNode obj3;
		Render::Camera* camera;
		Display::Window* window;
		float time;
		float prev_time;
		float dt;
	};

} // namespace Example