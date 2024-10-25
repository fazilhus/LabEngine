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
			glDeleteRenderbuffers(1, &rBuf);
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
			//pl.SetPos({ -6.25f, 1.25f, 1.4f });
			//lightManager.PushPointLight(pl);
			//pl.SetPos({ -6.25f, 1.25f, -2.2f });
			//lightManager.PushPointLight(pl);
			//pl.SetPos({ 4.9f, 1.25f, 1.4f });
			//lightManager.PushPointLight(pl);
			//pl.SetPos({ 4.9f, 1.25f, -2.2f });
			//lightManager.PushPointLight(pl);
			for (std::size_t i = 0; i < Render::MAX_NUM_LIGHT_SOURCES; ++i) {
				float x = Math::Random::rand_float(-2.5f, 2.5f);
				float z = Math::Random::rand_float(-2.5f, 2.5f);
				float r = Math::Random::rand_float();
				float g = Math::Random::rand_float();
				float b = Math::Random::rand_float();
				Math::vec3 c{ r, g, b };
				pl.SetPos({x, 0.5f, z});
				pl.SetAmbient(c * 0.01f);
				pl.SetDiffuse(c * 0.01f);
				pl.SetSpecular(c * 0.3f);
				lightManager.PushPointLight(pl);
			}

			this->camera = new Render::Camera(0.5f, 4.0f / 3.0f, 0.01f, 100.0f);
			this->camera->SetCameraPosition({ -13.0f, 2.0f, -0.5f });
			this->camera->SetSpeed(7.5f);
			this->camera->SetSens(0.05f);

			time = (float)glfwGetTime();
			prev_time = 0;
			dt = time - prev_time;

			glGenFramebuffers(1, &gBuf);
			glBindFramebuffer(GL_FRAMEBUFFER, gBuf);

			glGenTextures(1, &gPos);
			glBindTexture(GL_TEXTURE_2D, gPos);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, S_WIDTH, S_HEIGHT,
				0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D, gPos, 0);

			glGenTextures(1, &gDiffSpec);
			glBindTexture(GL_TEXTURE_2D, gDiffSpec);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, S_WIDTH, S_HEIGHT,
				0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
				GL_TEXTURE_2D, gDiffSpec, 0);

			glGenTextures(1, &gNorm);
			glBindTexture(GL_TEXTURE_2D, gNorm);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, S_WIDTH, S_HEIGHT,
				0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
				GL_TEXTURE_2D, gNorm, 0);

			GLuint att[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
			glDrawBuffers(3, att);

			glGenRenderbuffers(1, &rBuf);
			glBindRenderbuffer(GL_RENDERBUFFER, rBuf);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, S_WIDTH, S_HEIGHT);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
				GL_RENDERBUFFER, rBuf);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Framebuffer not complete!" << '\n';
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			const auto& s = shaderManager.Get("lPass").lock();
			s->Use();
			s->UploadUniform1i("gPos", 0);
			s->UploadUniform1i("gDiffSpec", 1);
			s->UploadUniform1i("gNorm", 2);
			s->UnUse();

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
			this->window->Update();
			HandleInput();

			angle += dt;
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuf);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			for (const auto& node : nodes) {
				node.Draw(*camera);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

} // namespace Example