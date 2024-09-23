//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2022 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include "imgui.h"

#include "render/mesh.h"
#include "math/mat4.h"
#include "input/input.h"

#include <iostream>

constexpr int STRING_BUFFER_SIZE = 8192;

using namespace Display;
namespace Example {

	//------------------------------------------------------------------------------
	/**
	*/
	ImGuiExampleApp::ImGuiExampleApp() {
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	ImGuiExampleApp::~ImGuiExampleApp() {
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void ImGuiExampleApp::Close() {
		if (this->window->IsOpen()) {
			this->window->Close();
		}

		Input::InputManager::Destroy();
		Core::App::Close();
		//this->mesh.DeInit();
		//this->shader.reset();
		this->cube.DeInit();
		delete this->grid;
		delete this->camera;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	bool ImGuiExampleApp::Open() {
		Input::InputManager::Create();
		App::Open();
		this->window = new Display::Window;
		window->SetKeyPressFunction(&Input::InputManager::KeyCallback);
		window->SetMousePressFunction(&Input::InputManager::MouseKeyCallback);
		window->SetMouseMoveFunction(&Input::InputManager::MouseMoveCallback);

		if (this->window->Open()) {
			glfwSwapInterval(1);
			// set clear color to gray
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			time = (float)glfwGetTime();
			prev_time = 0;

			this->cube = Resource::GraphicsNode(
				"../projects/GraphicsNodeExample/res/meshes/bunny.obj",
				"../projects/GraphicsNodeExample/res/textures/minecraft-dirt.png",
				"../projects/GraphicsNodeExample/res/shaders/vertex.glsl",
				"../projects/GraphicsNodeExample/res/shaders/fragment.glsl"
			);
			this->cube.transform *= Math::translate({0, 0, 0});

			this->camera = new Render::Camera(0.5f, 4.0f / 3.0f, 0.01f, 100.0f);
			this->camera->SetCameraPosition({0.0f, 2.0f, -2.0f});
			this->camera->SetSpeed(10.0f);
			this->camera->SetSens(0.05f);

			this->grid = new Render::Grid();

			return true;
		}
		return false;
	}





	//------------------------------------------------------------------------------
	/**
	*/
	void ImGuiExampleApp::Run() {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Math::mat4 gridT{};
		Math::mat4 meshT = Math::scale(0.5f);

		while (this->window->IsOpen()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->window->Update();
			HandleInput();
		
			float angle = (float)glfwGetTime();

			auto v = this->camera->GetView();
			auto p = this->camera->GetPerspective();

			this->cube.Draw(*camera);
			this->grid->Draw(&gridT[0][0], &v[0][0], &p[0][0]);

			// transfer new frame to window
			this->window->SwapBuffers();

			// delta time
			prev_time = time;
			time = (float)glfwGetTime();
			dt = time - prev_time;

			Input::InputManager::Flush();

#ifdef CI_TEST
			// if we're running CI, we want to return and exit the application after one frame
			// break the loop and hopefully exit gracefully
			break;
#endif
		}
	}

	void ImGuiExampleApp::HandleInput() {
		using namespace Input;
		if (InputManager::IsKeyPressed(Key::Escape)) {
			this->window->Close();
		}

		this->camera->UpdateCamera(dt);
	}

} // namespace Example