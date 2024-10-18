//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2022 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include "imgui.h"

#include "render/mesh.h"
#include "math/math.h"
#include "input/input.h"
#include "render/model.h"

#include <iostream>

#include "fx/gltf.h"

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
		//this->obj1.DeInit();
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

			std::filesystem::path resPath{ "../projects/GLTFExample/res" };
			resPath.make_preferred();

			shaderManager.Push(
				"lightSourceShader", 
				(resPath / "shaders/lightVert.glsl").make_preferred(),
				(resPath / "shaders/lightFrag.glsl").make_preferred());
			shaderManager.Push(
				"defaultShader",
				(resPath / "shaders/vertex.glsl").make_preferred(),
				(resPath / "shaders/fragment.glsl").make_preferred());
			shaderManager.Push(
				"BlinnPhongShader",
				(resPath / "shaders/blinnVertex.glsl").make_preferred(),
				(resPath / "shaders/blinnFragment.glsl").make_preferred());
			shaderManager.Push(
				"BlinnPhongNormalShader",
				(resPath / "shaders/blinnNormVertex.glsl").make_preferred(),
				(resPath / "shaders/blinnNormFragment.glsl").make_preferred());

			Resource::OBJMeshBuilder meshBuilder{ (resPath / "meshes/cube.obj").make_preferred() };
			auto cubeMesh = std::make_shared<Resource::Mesh>(meshBuilder.CreateMesh());

			cube = Resource::Model(
				resPath / std::filesystem::path("models/Cube/gltf/Cube.gltf").make_preferred(),
				shaderManager);
			cube.transform *= Math::translate({2.5f, 0.0f, 0.0f});
			avocado = Resource::Model(
				resPath / std::filesystem::path("models/Avocado/gltf/Avocado.gltf").make_preferred(),
				shaderManager);
			avocado.transform *= Math::scale(10.0f) * Math::rotationy(Math::toRad(180.0f));

			lightManager.PushLightingShader(shaderManager.Get("BlinnPhongShader"));
			lightManager.PushLightingShader(shaderManager.Get("BlinnPhongNormalShader"));
			lightManager.SetLightSourceShader(shaderManager.Get("lightSourceShader"));
			lightManager.SetMesh(cubeMesh);

			Render::DirectionalLight dl;
			dl.SetAmbient(Math::vec3(0.01f));
			dl.SetDiffuse(Math::vec3(0.1f));
			dl.SetSpecular(Math::vec3(0.4f));
			lightManager.SetGlobalLight(dl);

			Render::PointLight pl;
			pl.SetPos({ 0.0f, 0.0f, 5.0f });
			pl.SetAmbient(Math::vec3({0.05f, 0.0f, 0.05f}));
			pl.SetDiffuse(Math::vec3({0.2f, 0.0f, 0.2f}));
			pl.SetSpecular(Math::vec3({0.5f, 0.0f, 0.5f}));
			pl.SetAttenuation({ 1.0f, 0.022f, 0.019f });
			lightManager.PushPointLight(pl);

			Render::SpotLight sl;
			sl.SetPos({ 2.5f, 2.5f, 2.5f });
			sl.SetDirection(sl.GetPos() - Math::vec3{ 0.0f, 0.0f, 0.0f });
			sl.SetAmbient(Math::vec3({ 0.0f, 0.05f, 0.05f }));
			sl.SetDiffuse(Math::vec3({ 0.0f, 0.2f, 0.2f }));
			sl.SetSpecular(Math::vec3({ 0.0f, 0.5f, 0.5f }));
			sl.SetCutoffAngle(Math::toRad(15.0f));
			sl.SetOuterCutoffAngle(Math::toRad(20.0f));
			sl.SetAttenuation({ 1.0f, 0.09f, 0.032f });
			lightManager.PushSpotLight(sl);

			this->camera = new Render::Camera(0.5f, 4.0f / 3.0f, 0.01f, 100.0f);
			this->camera->SetCameraPosition({ 0.0f, 0.0f, 10.0f });
			this->camera->SetSpeed(7.5f);
			this->camera->SetSens(0.05f);

			time = (float)glfwGetTime();
			prev_time = 0;
			dt = time - prev_time;

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
		glEnable(GL_FRAMEBUFFER_SRGB);

		float angle = 0.0f;

		while (this->window->IsOpen()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->window->Update();
			HandleInput();

			angle += dt;
			auto& slight = this->lightManager.GetSpotLights()[0];
			slight.SetPos({cosf(angle) * 2.5f, 1.0f, sinf(angle) * 2.5f});
			slight.SetDirection(slight.GetPos() - Math::vec3{0.0f, 0.0f, 0.0f});

			lightManager.SetLightUniforms();

			//this->obj1.Draw(*camera);
			//this->obj2.Draw(*camera);
			//this->obj3.Draw(*camera);

			cube.Draw(*camera);
			avocado.Draw(*camera);

			lightManager.DrawLightSources(*camera);

			// transfer new frame to window
			this->window->SwapBuffers();

			// delta time
			prev_time = time;
			time = static_cast<float>(glfwGetTime());
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
		if (InputManager::IsKeyPressed(Key::Escape))
		{
			this->window->Close();
		}

		if (InputManager::IsKeyPressed(Key::LeftControl) && InputManager::IsKeyPressed(Key::R))
		{
			shaderManager.RecompileAll();
		}

		this->camera->UpdateCamera(dt);
	}

} // namespace Example