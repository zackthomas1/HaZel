#include "hzpch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazzel {
	PerspectiveCamera::PerspectiveCamera(float aspectRatio, float fov, float nearClip, float farClip)
		:m_Fov(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
	{

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_LookDirection, glm::vec3(0.0f, 1.0f, 0.0f));
		m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_NearClip, m_FarClip);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		m_LookDirection.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_LookDirection.y = sin(glm::radians(m_Pitch));
		m_LookDirection.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_LookDirection = glm::normalize(m_LookDirection);

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_LookDirection, glm::vec3(0.0f, 1.0f, 0.0f));
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::RecalculateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_NearClip, m_FarClip);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}