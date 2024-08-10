#include "Camera.h"
#include "GLFW/glfw3.h"

namespace lf {
	Camera* activeCamera = nullptr;

	Camera* Camera::GetActiveCamera()
	{
		return activeCamera;
	}

	void Camera::activate()
	{
		activeCamera = this;
	}
}