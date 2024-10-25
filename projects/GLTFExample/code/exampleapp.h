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
		std::shared_ptr<Resource::Model> helmetModel;
		Render::Camera* camera;
		Display::Window* window;
		float time;
		float prev_time;
		float dt;

		GLuint gBuf, rBuf;
		GLuint gPos, gDiffSpec, gNorm;

		std::vector<Resource::GraphicsNode> nodes;

		GLuint quadVAO = 0, quadVBO = 0;

		void renderQuad();
	};

} // namespace Example