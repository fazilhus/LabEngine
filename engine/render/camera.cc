#include "config.h"
#include "camera.h"

#include "input/input.h"
#include "math/math.h"

#include <iostream>

namespace Render {
	
	Camera::Camera(const float fovy, const float aspect, const float near, const float far)
		: pos({ 0, 0, 0 }), at(pos + Math::vec3{ 0, 0, 1 }), up({ 0, 1, 0 }),
		yaw(-90.0f), pitch(0), speed(25.0f), sens(0.15f) {
		perspective = Math::perspective(fovy, aspect, near, far);
		UpdateView();
	}

	void Camera::UpdateCamera(float dt, Display::Window& win) {
		using namespace Input;
		if (InputManager::IsMouseButtonPressed(MouseKey::ButtonRight)) {
			Math::vec4 f{};
			if (InputManager::IsKeyPressed(Key::W)) {
				f.z -= speed * dt;
			}
			if (InputManager::IsKeyPressed(Key::S)) {
				f.z += speed * dt;
			}
			if (InputManager::IsKeyPressed(Key::A)) {
				f.x -= speed * dt;
			}
			if (InputManager::IsKeyPressed(Key::D)) {
				f.x += speed * dt;
			}
			if (InputManager::IsKeyPressed(Key::Q)) {
				f.y += speed * dt;
			}
			if (InputManager::IsKeyPressed(Key::E)) {
				f.y -= speed * dt;
			}
			auto pos = f * Math::inverse(view);
			this->pos += {pos.x, pos.y, pos.z};
		}

		if (InputManager::IsMouseButtonPressed(MouseKey::ButtonMiddle)) {
			auto md = InputManager::GetMouseDeltaPos();
			yaw += sens * md.x * dt;
			pitch -= sens * md.y * dt;
			if (pitch > PI_OVER_TWO) {
				pitch = PI_OVER_TWO - 0.0001f;
			}
			else if (pitch < -PI_OVER_TWO) {
				pitch = -PI_OVER_TWO + 0.0001f;
			}
		}
		UpdateView();
	}

	void Camera::SetCameraPosition(const Math::vec3& v, bool update) {
		pos = v;
		if (update) UpdateView();
	}

	void Camera::SetLookatPosition(const Math::vec3& v, bool update) {
		at = v;
		if (update) UpdateView();
	}

	void Camera::SetUpDirection(const Math::vec3& v, bool update) {
		up = v;
		if (update) UpdateView();
	}

	void Camera::UpdateView() {
		//view = Math::lookat(pos, at, up);
		Math::vec3 f = Math::normalize({
			cosf(yaw) * cosf(pitch),
			sinf(pitch),
			cosf(pitch) * sinf(yaw)
		});
		at = pos + f;
		view = Math::lookat(pos, at, up);
	}

} // Render