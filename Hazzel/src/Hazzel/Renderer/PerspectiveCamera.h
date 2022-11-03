#pragma once
#include <glm/glm.hpp>

namespace Hazzel {
	class PerspectiveCamera
	{
	public:
		PerspectiveCamera(float aspectRatio = 16.f / 9.f, float fov = 45.0f, float nearClip = 0.1f, float farClip = 100.f);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		const glm::vec3& GetLookDirection() const { return m_LookDirection; }
		void SetLookDirection(const glm::vec3& lookDirection) { m_LookDirection = lookDirection; RecalculateViewMatrix(); }

		const glm::vec3& GetUpDirection() const { return m_UpDirection; }
		void SetUpDirection(const glm::vec3& upDirection) { m_UpDirection = upDirection; RecalculateViewMatrix(); }

		const float GetPitch() const { return m_Pitch; }
		void SetPitch(float pitch) 
		{ 
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			m_Pitch = pitch; 
			RecalculateViewMatrix(); 
		}

		const float GetYaw() const { return m_Yaw; }
		void SetYaw(float yaw) { m_Yaw = yaw; RecalculateViewMatrix(); }

		const float GetFOV() const { return m_Fov; }
		void SetFOV(float fov) 
		{
			if (fov < 1.0f)
				fov = 1.0f;
			else if (fov > 45.0f)
				fov = 45.0f;

			m_Fov = fov; 
			RecalculateProjectionMatrix(); 
		}

		const glm::mat4& GetViewMatrix()			const { return m_ViewMatrix; }
		const glm::mat4& GetProjectionMatrix()		const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewProjectionMatrix()	const { return m_ViewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();
		void RecalculateProjectionMatrix();
	private:
		glm::mat4 m_ViewMatrix, m_ProjectionMatrix, m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 3.0f };
		glm::vec3 m_LookDirection = { 0.0f, 0.0f, -1.0f };
		glm::vec3 m_UpDirection = { 0.0f, 1.0f, 0.0f };

		float m_Pitch = 0.0f, m_Yaw = -90.0f;

		float m_Fov;
		float m_AspectRatio; 
		float m_NearClip, m_FarClip;
	};
}