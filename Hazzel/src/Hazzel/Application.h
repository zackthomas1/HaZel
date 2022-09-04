#pragma once

#include "Core.h"

#include "Window.h"
#include "Hazzel/LayerStack.h"
#include "Events/Event.h"
#include "Hazzel/Events/ApplicationEvent.h"

namespace Hazzel {
	class HAZZEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer); 
		void PushOverlay(Layer* overlay);

	private: 
		bool OnWindowClose(WindowCloseEvent& e); 

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	// to be defined in CLIENT
	Application* CreateApplication();
}

