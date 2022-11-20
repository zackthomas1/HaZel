#pragma once
#include "Hazzel/Renderer/OrthographicCamera.h"
 
#include "Hazzel/Core/TimeStep.h"
#include "Hazzel/Events/ApplicationEvent.h"
#include "Hazzel/Events/MouseEvent.h"

namespace Hazzel
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectratio, bool isRotatable = false);

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& event);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);

	private:

		float m_AspectRatio; 
		float m_ZoomLevel = 1.0f;
		bool m_IsRotatable; 
		
		float m_CameraTranslationSpeed = 2.0f, m_CameraRotationSpeed = 180.f, m_CameraZoomSpeed = 0.25f;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;

		OrthographicCamera m_Camera;
	};
}