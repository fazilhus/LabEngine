#include "config.h"
#include "input.h"

#include <GLFW/glfw3.h>

#include <cassert>
#include <iostream>

namespace Input {
	
	struct InputDevices {
		Keyboard keyboard;
		Mouse mouse;
	};

	static InputDevices* inputDevices = nullptr;

	void InputManager::Create() {
		if (!inputDevices)
			inputDevices = new InputDevices;
	}

	void InputManager::Destroy() {
		if (inputDevices) {
			delete inputDevices;
			inputDevices = nullptr;
		}
	}

	void InputManager::Flush() {
		auto& k = inputDevices->keyboard;
		for (std::size_t i = 0; i < Key::NumOfKeys; ++i) {
			if (k.released[i]) {
				k.held[i] = false;
			}
			k.pressed[i] = false;
			k.released[i] = false;
		}

		auto& m = inputDevices->mouse;
		for (std::size_t i = 0; i < MouseKey::NumOfMouseKeys; ++i) {
			if (m.released[i]) {
				m.held[i] = false;
			}
			m.pressed[i] = false;
			m.released[i] = false;
		}
		
		m.prev_pos = m.pos;
		m.delta = Math::vec2(0);
	}

	void InputManager::KeyCallback(int32 key, int32 scancode, int32 action, int32 mods) {
		auto& k = inputDevices->keyboard;
		auto code = (KeyCode)key;
		switch (action) {
		case GLFW_PRESS:
			k.pressed[code] = true;
			k.held[code] = true;
			break;
		case GLFW_RELEASE:
			k.released[code] = true;
			break;
		}
	}

	void InputManager::MouseKeyCallback(int32 button, int32 action, int32 mods) {
		auto& m = inputDevices->mouse;
		auto code = (MouseKeyCode)button;
		switch (action) {
		case GLFW_PRESS:
			m.pressed[code] = true;
			m.held[code] = true;
			break;
		case GLFW_RELEASE:
			m.released[code] = true;
		}
	}

	void InputManager::MouseMoveCallback(float32 x, float32 y) {
		auto& m = inputDevices->mouse;
		m.pos = Math::vec2(x, y);
		m.delta = m.pos - m.prev_pos;
	}

	bool InputManager::IsKeyJustPressed(KeyCode code) {
		return inputDevices->keyboard.pressed[code];
	}

	bool InputManager::IsKeyPressed(KeyCode code) {
		return IsKeyJustPressed(code) || inputDevices->keyboard.held[code];
	}

	bool InputManager::IsKeyReleased(KeyCode code) {
		return inputDevices->keyboard.released[code];
	}

	bool InputManager::IsMouseButtonJustPressed(MouseKeyCode code) {
		return inputDevices->mouse.pressed[code];
	}

	bool InputManager::IsMouseButtonPressed(MouseKeyCode code) {
		return IsMouseButtonJustPressed(code) || inputDevices->mouse.held[code];
	}

	bool InputManager::IsMouseButtonReleased(MouseKeyCode code) {
		return inputDevices->mouse.released[code];
	}

	Math::vec2 InputManager::GetMousePos() {
		return inputDevices->mouse.pos;
	}

	Math::vec2 InputManager::GetMouseDeltaPos() {
		return inputDevices->mouse.delta;
	}

} // Input