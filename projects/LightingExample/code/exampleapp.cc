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
#include "math/math.h"
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
		this->obj1.DeInit();
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
			glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
			time = (float)glfwGetTime();
			prev_time = 0;

			auto catDiffTex = std::make_shared<Resource::Texture>(
				"../projects/LightingExample/res/textures/cat_diff.tga", 1);
			auto catSpecTex = std::make_shared<Resource::Texture>(
				"../projects/LightingExample/res/textures/cat_spec.tga", 1);
			auto catMat = std::make_shared<Resource::Material>(catDiffTex, catSpecTex, 4.0f);

			Resource::MeshBuilder meshBuilder{ "../projects/LightingExample/res/meshes/cat.obj" };
			auto catMesh = std::make_shared<Resource::Mesh>(meshBuilder.CreateMesh());

			auto shader = std::make_shared<Resource::Shader>(
				"../projects/LightingExample/res/shaders/vertex.glsl",
				"../projects/LightingExample/res/shaders/fragment.glsl");

			this->obj1 = Resource::GraphicsNode();
			this->obj1.SetMaterial(catMat);
			this->obj1.SetMesh(catMesh);
			this->obj1.SetShader(shader);
			this->obj1.transform *= Math::translate({1, 0, 1});


			Render::DirectionalLight dl;
			dl.SetAmbient(Math::vec3(0.01f));
			lm.SetGlobalLight(dl);

			Render::PointLight pl;
			pl.SetPos({ 0.0f, 0.0f, 10.0f });
			pl.SetAttenuation({ 1.0f, 0.022f, 0.019f });
			lm.PushPointLight(pl);

			Render::SpotLight sl;
			sl.SetPos({ 10.0f, 2.0f, 0.0f });
			sl.SetDirection(sl.GetPos() - Math::vec3{ 0.0f, 0.0f, 0.0f });
			sl.SetCutoffAngle(Math::toRad(15.0f));
			sl.SetOuterCutoffAngle(Math::toRad(20.0f));
			sl.SetAttenuation({ 1.0f, 0.09f, 0.032f });
			lm.PushSpotLight(sl);

			this->camera = new Render::Camera(0.5f, 4.0f / 3.0f, 0.01f, 100.0f);
			this->camera->SetCameraPosition({ 0.0f, 0.0f, 10.0f });
			this->camera->SetSpeed(7.5f);
			this->camera->SetSens(0.05f);

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

		float angle = 0.0f;

		while (this->window->IsOpen()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->window->Update();
			HandleInput();

			angle += dt;
			//auto& slight = this->lm.GetSpotLights()[0];
			//slight.SetPos({cosf(angle) * 10.0f, 2.0f, sinf(angle) * 10.0f});
			//slight.SetDirection(slight.GetPos() - Math::vec3{0.0f, 0.0f, 0.0f});

			this->obj1.GetShader().Use();
			lm.SetLightUniforms(this->obj1.GetShader());

			this->obj1.Draw(*camera);
			//this->obj2.Draw(*camera);
			//this->obj3.Draw(*camera);

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