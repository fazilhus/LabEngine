#pragma once

#include "math/vec3.h"
#include "math/mat4.h"

namespace Render {

    class Camera{
    private:
        Math::mat4 perspective;
        Math::mat4 view;

        Math::vec3 pos;
        Math::vec3 at;
        Math::vec3 up;

    public:
        Camera(const float fovy, const float aspect, const float near, const float far);
        ~Camera() = default;

        void SetCameraPosition(const Math::vec3& v);
        void SetLookatPosition(const Math::vec3& v);
        void SetUpDirection(const Math::vec3& v);

        inline Math::mat4 GetPerspective() const { return perspective; }
        inline Math::mat4 GetView() const { return view; }

    private:
        void UpdateView();
    };

} // Render