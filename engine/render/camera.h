#pragma once

#include "math/vec3.h"
#include "math/mat4.h"
#include "render/window.h"

namespace Render {

    class Camera{
    private:
        Math::mat4 perspective;
        Math::mat4 view;

        Math::vec3 pos;
        Math::vec3 at;
        Math::vec3 up;

        float yaw;
        float pitch;

        float speed;
        float sens;

    public:
        Camera(const float fovy, const float aspect, const float near, const float far);
        ~Camera() = default;

        void UpdateCamera(float dt);

        void SetCameraPosition(const Math::vec3& v, bool update = true);
        void SetLookatPosition(const Math::vec3& v, bool update = true);
        void SetUpDirection(const Math::vec3& v, bool update = true);
        inline void SetSpeed(float speed) { this->speed = speed; }
        inline void SetSens(float sens) { this->sens = sens; }

        inline const Math::vec3& GetCameraPos() const { return pos; }
        inline const Math::vec3& GetCameraAt() const { return at; }
        inline const Math::vec3& GetCameraUp() const { return up; }

        inline Math::mat4 GetPerspective() const { return perspective; }
        inline Math::mat4 GetView() const { return view; }

    private:
        void UpdateView();
    };

} // Render