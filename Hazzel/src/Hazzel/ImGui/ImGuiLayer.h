#pragma once

#include "Hazzel/Layer.h"

#include "Hazzel/Events/ApplicationEvent.h"
#include "Hazzel/Events/KeyEvent.h"
#include "Hazzel/Events/MouseEvent.h"

namespace Hazzel {

	class HAZZEL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(); 
		~ImGuiLayer(); 

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& event) override;

	private: 
		bool OnWindowResizeEvent(WindowResizeEvent& e);
		
		bool OnMouseButtonPressedEvent(MouseButtonEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);

		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);

	private: 
		float m_Time = 0.f;
	};
}