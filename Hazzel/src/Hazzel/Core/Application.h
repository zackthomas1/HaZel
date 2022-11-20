#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Hazzel/Events/Event.h"
#include "Hazzel/Events/ApplicationEvent.h"

#include "Hazzel/ImGui/ImGuiLayer.h"

namespace Hazzel {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer); 
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& getWindow() { return *m_Window; }
	private: 
		bool OnWindowClose(WindowCloseEvent& e); 
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;

		float m_LastTimeFrame = 0.0f;


	private:
		static Application* s_Instance;
	};

	// to be defined in CLIENT
	Application* CreateApplication();
}

