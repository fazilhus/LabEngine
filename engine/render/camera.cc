#include "config.h"
#include "camera.h"

namespace Render {
	
	Camera::Camera(const float fovy, const float aspect, const float near, const float far)
		: pos({ 0, 0, 0 }), at(pos + Math::vec3{ 0, 0, 1 }), up({ 0, 1, 0 }) {
		perspective = Math::perspective(fovy, aspect, near, far);
		UpdateView();
	}

	void Camera::SetCameraPosition(const Math::vec3& v) {
		pos = v;
		UpdateView();
	}

	void Camera::SetLookatPosition(const Math::vec3& v) {
		at = v;
		UpdateView();
	}

	void Camera::SetUpDirection(const Math::vec3& v) {
		up = v;
		UpdateView();
	}

	void Camera::UpdateView() {
		view = Math::lookat(pos, at, up);
	}

} // Render