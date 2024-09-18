#pragma once

#include "math/vec2.h"
#include "input/codes.h"

#include <vector>
#include <memory>

namespace Input {

	struct Keyboard {
		std::vector<bool> pressed = std::vector<bool>(Key::NumOfKeys);
		std::vector<bool> released = std::vector<bool>(Key::NumOfKeys);
		std::vector<bool> held = std::vector<bool>(Key::NumOfKeys);
	};

	struct Mouse {
		std::vector<bool> pressed = std::vector<bool>(MouseKey::NumOfMouseKeys);
		std::vector<bool> released = std::vector<bool>(MouseKey::NumOfMouseKeys);
		std::vector<bool> held = std::vector<bool>(MouseKey::NumOfMouseKeys);

		Math::vec2 pos;
		Math::vec2 prev_pos;
		Math::vec2 delta;
	};

	class InputManager {
	public:
		InputManager() = delete;
		~InputManager() = delete;

		static void Create();
		static void Destroy();

		static void Flush();

		static void KeyCallback(int32 key, int32 scancode, int32 action, int32 mods);
		static void MouseKeyCallback(int32 button, int32 action, int32 mods);
		static void MouseMoveCallback(float32 x, float32 y);

		static bool IsKeyPressed(KeyCode code);
		static bool IsKeyReleased(KeyCode code);
		static bool IsKeyHeld(KeyCode code);

		static bool IsMouseButtonPressed(MouseKeyCode code);
		static bool IsMouseButtonReleased(MouseKeyCode code);
		static bool IsMouseButtonHeld(MouseKeyCode code);

		static Math::vec2 GetMousePos();
		static float GetMousePosX();
		static float GetMousePosY();
	};

} // Input