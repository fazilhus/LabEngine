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
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			time = (float)glfwGetTime();
			prev_time = 0;

			auto dirtTex = std::make_shared<Resource::Texture>("../projects/LightingExample/res/textures/minecraft-dirt.png");
			auto ballTex = std::make_shared<Resource::Texture>("../projects/LightingExample/res/textures/ball.png");

			auto goldMat = std::make_shared<Resource::Material>(Resource::Material{
				{ 0.24725f , 0.1995f  , 0.0745f   },
				{ 0.75164f , 0.60648f , 0.22648f  },
				{ 0.628281f, 0.555802f,	0.366065f },
				0.4f});
			auto pearlMat = std::make_shared<Resource::Material>(Resource::Material{
				{     0.25f, 0.20725f , 0.20725f  },
				{     1.0f , 0.829f   , 0.829f    },
				{ 0.296648f, 0.296648f,	0.296648f },
				0.088f });

			Resource::MeshBuilder meshBuilder{ "../projects/LightingExample/res/meshes/bunny.obj" };
			auto bunnyMesh = std::make_shared<Resource::Mesh>(meshBuilder.CreateMesh());

			meshBuilder.ReadMeshData("../projects/LightingExample/res/meshes/sphere.obj");
			auto sphereMesh = std::make_shared<Resource::Mesh>(meshBuilder.CreateMesh());

			meshBuilder.ReadMeshData("../projects/LightingExample/res/meshes/cube.obj");
			auto cubeMesh = std::make_shared<Resource::Mesh>(meshBuilder.CreateMesh());

			auto shader = std::make_shared<Resource::Shader>(
				"../projects/LightingExample/res/shaders/vertex.glsl",
				"../projects/LightingExample/res/shaders/fragment.glsl");

			this->obj1 = Resource::GraphicsNode();
			this->obj1.SetTexture(dirtTex);
			this->obj1.SetMesh(bunnyMesh);
			this->obj1.SetMaterial(goldMat);
			this->obj1.SetShader(shader);
			this->obj1.transform *= Math::translate({1, 0, 1});

			this->obj2 = Resource::GraphicsNode();
			this->obj2.SetTexture(ballTex);
			this->obj2.SetMesh(sphereMesh);
			this->obj2.SetMaterial(pearlMat);
			this->obj2.SetShader(shader);
			this->obj2.transform *= Math::translate({ -1, 0, -1 });

			this->obj3 = Resource::GraphicsNode();
			this->obj3.SetTexture(ballTex);
			this->obj3.SetMesh(cubeMesh);
			this->obj3.SetMaterial(pearlMat);
			this->obj3.SetShader(shader);
			this->obj3.transform *= Math::translate({ 0, 2, 2 });
			this->obj3.transform *= Math::scale(0.5f);

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
			auto& slight = this->obj1.GetShader().GetSpotLight();
			slight.SetPos({cosf(angle) * 10.0f, 2.0f, sinf(angle) * 10.0f});
			slight.SetDirection(slight.GetPos() - Math::vec3{0.0f, 0.0f, 0.0f});

			this->obj1.Draw(*camera);
			this->obj2.Draw(*camera);
			this->obj3.Draw(*camera);

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