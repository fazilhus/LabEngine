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
			helmetModel->UnLoad();
			helmetModel.reset();
			sponzaModel->UnLoad();
			sponzaModel.reset();
			glDeleteVertexArrays(1, &quadVAO);
			glDeleteBuffers(1, &quadVBO);

			this->window->Close();
		}

		Input::InputManager::Destroy();
		Core::App::Close();

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

			std::filesystem::path resPath{ "../projects/GLTFExample/res" };
			resPath.make_preferred();

			shaderManager.Push(
				"lightSourceShader",
				(resPath / "shaders/debugVert.glsl").make_preferred(),
				(resPath / "shaders/debugFrag.glsl").make_preferred());
			shaderManager.Push(
				"gPass",
				(resPath / "shaders/gVert.glsl").make_preferred(),
				(resPath / "shaders/gFrag.glsl").make_preferred());
			shaderManager.Push(
				"gNormPass",
				(resPath / "shaders/gNormVert.glsl").make_preferred(),
				(resPath / "shaders/gNormFrag.glsl").make_preferred());
			shaderManager.Push(
				"PointLightPass",
				(resPath / "shaders/LightVert.glsl").make_preferred(),
				(resPath / "shaders/PointLightFrag.glsl").make_preferred());
			shaderManager.Push(
				"DirLightPass",
				(resPath / "shaders/LightVert.glsl").make_preferred(),
				(resPath / "shaders/DirLightFrag.glsl").make_preferred());
			shaderManager.Push(
				"nullShader",
				(resPath / "shaders/nVert.glsl").make_preferred(),
				(resPath / "shaders/nFrag.glsl").make_preferred());

			Resource::OBJMeshBuilder meshBuilder{ (resPath / "meshes/sphere.obj").make_preferred() };
			auto debugSphere = std::make_shared<Resource::Mesh>(meshBuilder.CreateMesh());

			helmetModel = std::make_shared<Resource::Model>(
				resPath / std::filesystem::path("models/FlightHelmet/gltf/FlightHelmet.gltf").make_preferred(),
				shaderManager
			);
			sponzaModel = std::make_shared<Resource::Model>(
				resPath / std::filesystem::path("models/Sponza/gltf/Sponza.gltf").make_preferred(),
				shaderManager
			);

			for (std::size_t i = 0; i < 5; ++i) {
				for (std::size_t j = 0; j < 5; ++j) {
					Resource::GraphicsNode node{ helmetModel };
					node.transform *= Math::translate({-2.5f + i * 1.0f, 0.0f, -2.5f + j * 1.0f})
						* Math::rotationy(Math::toRad(Math::Random::rand_int(-180, 180)));
					nodes.push_back(node);
				}
			}
			nodes.emplace_back(sponzaModel);
			nodes.back().transform *= Math::scale(0.015f);


			//lightManager.PushLightingShader(shaderManager.Get("PointLightPass"));
			lightManager.SetLightSourceShader(shaderManager.Get("lightSourceShader"));
			lightManager.SetMesh(debugSphere);

			Render::DirectionalLight dl;
			dl.SetDirection({ 0, -1, -1});
			dl.SetAmbient(Math::vec3(0.05f));
			dl.SetDiffuse(Math::vec3(0.1f));
			dl.SetSpecular(Math::vec3(0.3f));
			lightManager.SetGlobalLight(dl);

			Render::PointLight pl;
			pl.SetFalloff(200.0f);
			pl.SetRadius(45.0f);
			for (std::size_t i = 0; i < Render::MAX_NUM_LIGHT_SOURCES; ++i) {
				float x = Math::Random::rand_float(-19.0f, 24.0f);
				float y = Math::Random::rand_float(0.5f, 12.5f);
				float z = Math::Random::rand_float(-11.0f, 9.0f);
				float r = Math::Random::rand_float();
				float g = Math::Random::rand_float();
				float b = Math::Random::rand_float();
				Math::vec3 c{ r, g, b };
				pl.SetPos({x, y, z});
				pl.SetAmbient(c * 0.005f);
				pl.SetDiffuse(c * 0.01f);
				pl.SetSpecular(c * 0.25f);
				lightManager.PushPointLight(pl);
			}

			this->camera = new Render::Camera(0.5f, 4.0f / 3.0f, 0.01f, 100.0f);
			this->camera->SetCameraPosition({ -13.0f, 2.0f, -0.5f });
			this->camera->SetSpeed(7.5f);
			this->camera->SetSens(0.05f);

			time = (float)glfwGetTime();
			prev_time = 0;
			dt = time - prev_time;

			gbuf.Init(S_WIDTH, S_HEIGHT);

			{
				const auto& s = shaderManager.Get("PointLightPass").lock();
				s->Use();
				s->UploadUniform1i("gPos", 0);
				s->UploadUniform1i("gCol", 1);
				s->UploadUniform1i("gNorm", 2);
				s->UnUse();
			}

			{
				const auto& s = shaderManager.Get("DirLightPass").lock();
				s->Use();
				s->UploadUniform1i("gPos", 0);
				s->UploadUniform1i("gCol", 1);
				s->UploadUniform1i("gNorm", 2);
				s->UnUse();
			}

			return true;
		}
		return false;
	}





	//------------------------------------------------------------------------------
	/**
	*/
	void ImGuiExampleApp::Run() {
		glEnable(GL_CULL_FACE);
		glEnable(GL_FRAMEBUFFER_SRGB);

		float angle = 0.0f;

		while (this->window->IsOpen()) {
			this->window->Update();
			HandleInput();

			angle += dt;

			gbuf.StartFrame();

			GeometryPass();

			glEnable(GL_STENCIL_TEST);
			for (const auto& [i, pl] : std::views::enumerate(lightManager.GetPointLights())) {
				StencilPassPointLight(pl);
				LightingPassPointLight(pl, i);
			}
			glDisable(GL_STENCIL_TEST);

			LightingPassGlobalLight();

			gbuf.BindForDebugPass();
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
			lightManager.DrawLightSources(*camera);

			FinalPass();

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

	void ImGuiExampleApp::renderQuad() {
		if (quadVAO == 0) {
			GLfloat quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};

			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(
				0,
				3,
				GL_FLOAT,
				GL_FALSE,
				5 * sizeof(GLfloat),
				(GLvoid*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(
				1,
				2,
				GL_FLOAT,
				GL_FALSE,
				5 * sizeof(GLfloat),
				(GLvoid*)(3 * sizeof(GLfloat)));
			glBindVertexArray(0);
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	void ImGuiExampleApp::GeometryPass() {
		const auto& s = shaderManager.Get("gNormPass").lock();
		s->Use();

		gbuf.BindForGeometryPass();

		glDepthMask(GL_TRUE);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		for (const auto& node : nodes) {
			node.Draw(*camera);
		}

		glDepthMask(GL_FALSE);

		s->UnUse();
	}

	void ImGuiExampleApp::StencilPassPointLight(const Render::PointLight& pl) {
		const auto& s = shaderManager.Get("nullShader").lock();
		s->Use();

		gbuf.BindForStencilPass();
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		glClearStencil(0);
		glClear(GL_STENCIL_BUFFER_BIT);
		glStencilFunc(GL_ALWAYS, 0x01, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		s->UploadUniformMat4fv("view", camera->GetView());
		s->UploadUniformMat4fv("perspective", camera->GetPerspective());

		auto transform = Math::translate(pl.GetPos()) * Math::scale(pl.GetRadius());
		s->UploadUniformMat4fv("transform", transform);

		lightManager.GetMesh()->Draw();

		s->UnUse();
	}

	void ImGuiExampleApp::LightingPassPointLight(const Render::PointLight& pl, std::size_t i) {
		const auto& s = shaderManager.Get("PointLightPass").lock();
		s->Use();

		gbuf.BindForLightingPass();

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		Math::vec2 dim{ S_WIDTH, S_HEIGHT };

		s->UploadUniform2fv("screen_dim", dim);
		s->UploadUniform3fv("cam_pos", camera->GetCameraPos());
		s->UploadUniform3fv("light.pos", pl.GetPos());
		s->UploadUniform3fv("light.ambient", pl.GetAmbient());
		s->UploadUniform3fv("light.diffuse", pl.GetDiffuse());
		s->UploadUniform3fv("light.specular", pl.GetSpecular());
		s->UploadUniform1f("light.radius", pl.GetRadius());
		s->UploadUniform1f("light.falloff", pl.GetFalloff());

		//s->UploadUniformMat4fv("view", camera->GetView());
		//s->UploadUniformMat4fv("perspective", camera->GetPerspective());

		auto transform = Math::translate(pl.GetPos()) * Math::scale(pl.GetRadius());
		s->UploadUniformMat4fv("mvp", camera->GetPerspective() * camera->GetView() * transform);
		lightManager.GetMesh()->Draw();

		glCullFace(GL_BACK);
		glDisable(GL_BLEND);

		s->UnUse();
	}

	void ImGuiExampleApp::LightingPassGlobalLight() {
		const auto& s = shaderManager.Get("DirLightPass").lock();
		s->Use();

		gbuf.BindForLightingPass();

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		Math::vec2 dim{ S_WIDTH, S_HEIGHT };
		const auto& l = lightManager.GetGlobalLight();

		s->UploadUniform2fv("screen_dim", dim);
		s->UploadUniform3fv("cam_pos", camera->GetCameraPos());
		s->UploadUniform3fv("light.dir", l.GetDirection());
		s->UploadUniform3fv("light.ambient", l.GetAmbient());
		s->UploadUniform3fv("light.diffuse", l.GetDiffuse());
		s->UploadUniform3fv("light.specular", l.GetSpecular());

		//s->UploadUniformMat4fv("view", camera->GetView());
		//s->UploadUniformMat4fv("perspective", camera->GetPerspective());

		s->UploadUniformMat4fv("mvp", Math::mat4::identity());

		renderQuad();
		glDisable(GL_BLEND);

		s->UnUse();
	}

	void ImGuiExampleApp::FinalPass() {
		gbuf.BindForFinalPass();
		glBlitFramebuffer(
			0, 0, S_WIDTH, S_HEIGHT,
			0, 0, S_WIDTH, S_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	}

} // namespace Example