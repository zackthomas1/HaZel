#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "ImGui/ImGuiLayer.h"

#include "Hazzel/Renderer/VertexArray.h"
#include "Hazzel/Renderer/Shader.h"
#include "Hazzel/Renderer/Buffer.h"

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

		inline static Application& Get() { return *s_Instance; }
		inline Window& getWindow() { return *m_Window; }
	private: 
		bool OnWindowClose(WindowCloseEvent& e); 

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

	private:
		static Application* s_Instance;
	};

	// to be defined in CLIENT
	Application* CreateApplication();
}

