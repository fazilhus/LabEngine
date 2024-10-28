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
			glDeleteFramebuffers(1, &gBuf);
			glDeleteTextures(1, &gPos);
			glDeleteTextures(1, &gDiffSpec);
			glDeleteTextures(1, &gNorm);
			glDeleteRenderbuffers(1, &gDepth);
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
			glClearColor(0.01f, 0.01f, 0.01f, 1.0f);

			std::filesystem::path resPath{ "../projects/GLTFExample/res" };
			resPath.make_preferred();

			shaderManager.Push(
				"lightSourceShader",
				(resPath / "shaders/lightVert.glsl").make_preferred(),
				(resPath / "shaders/lightFrag.glsl").make_preferred());
			shaderManager.Push(
				"gPass",
				(resPath / "shaders/gVert.glsl").make_preferred(),
				(resPath / "shaders/gFrag.glsl").make_preferred());
			shaderManager.Push(
				"gNormPass",
				(resPath / "shaders/gNormVert.glsl").make_preferred(),
				(resPath / "shaders/gNormFrag.glsl").make_preferred());
			shaderManager.Push(
				"lPass",
				(resPath / "shaders/lVert.glsl").make_preferred(),
				(resPath / "shaders/lFrag.glsl").make_preferred());
			shaderManager.Push(
				"nullShader",
				(resPath / "shaders/nVert.glsl").make_preferred(),
				(resPath / "shaders/nFrag.glsl").make_preferred());

			Resource::OBJMeshBuilder meshBuilder{ (resPath / "meshes/cube.obj").make_preferred() };
			auto cubeMesh = std::make_shared<Resource::Mesh>(meshBuilder.CreateMesh());

			helmetModel = std::make_shared<Resource::Model>(
				resPath / std::filesystem::path("models/FlightHelmet/gltf/FlightHelmet.gltf").make_preferred(),
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


			lightManager.PushLightingShader(shaderManager.Get("lPass"));
			lightManager.SetLightSourceShader(shaderManager.Get("lightSourceShader"));
			lightManager.SetMesh(cubeMesh);

			Render::DirectionalLight dl;
			dl.SetDirection({ 0, -1, -1});
			dl.SetAmbient(Math::vec3(0));
			dl.SetDiffuse(Math::vec3(0));
			dl.SetSpecular(Math::vec3(0));
			lightManager.SetGlobalLight(dl);

			Render::PointLight pl;
			pl.SetAttenuation({ 1.0f, 0.022f, 0.019f });
			for (std::size_t i = 0; i < Render::MAX_NUM_LIGHT_SOURCES; ++i) {
				float x = Math::Random::rand_float(-2.5f, 2.5f);
				float z = Math::Random::rand_float(-2.5f, 2.5f);
				float r = Math::Random::rand_float();
				float g = Math::Random::rand_float();
				float b = Math::Random::rand_float();
				Math::vec3 c{ r, g, b };
				pl.SetPos({x, 0.5f, z});
				pl.SetAmbient(c * 0.005f);
				pl.SetDiffuse(c * 0.01f);
				pl.SetSpecular(c * 0.15f);
				lightManager.PushPointLight(pl);
			}

			this->camera = new Render::Camera(0.5f, 4.0f / 3.0f, 0.01f, 100.0f);
			this->camera->SetCameraPosition({ -13.0f, 2.0f, -0.5f });
			this->camera->SetSpeed(7.5f);
			this->camera->SetSens(0.05f);

			time = (float)glfwGetTime();
			prev_time = 0;
			dt = time - prev_time;

			gbuf.Init(S_WIDTH, S_WIDTH);

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
			
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT);
			const auto& s = shaderManager.Get("lPass").lock();
			s->Use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gPos);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, gDiffSpec);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, gNorm);
			s->UploadUniform3fv("cam_pos", camera->GetCameraPos());
			lightManager.SetLightUniforms();

			s->Use();
			renderQuad();
			s->UnUse();

			glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuf);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glBlitFramebuffer(0, 0, S_WIDTH, S_HEIGHT, 0, 0, S_WIDTH, S_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_DEPTH_TEST);
			glDepthMask()
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

	void ImGuiExampleApp::StencilPass() {
		const auto& s = shaderManager.Get("nullShader").lock();
		s->Use();

		gbuf.BindForStencilPass();
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		glClear(GL_STENCIL_BUFFER_BIT);
		glStencilFunc(GL_ALWAYS, 0, 0);
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);



		s->UnUse();
	}

	void ImGuiExampleApp::LightingPass() {
		
	}

} // namespace Example