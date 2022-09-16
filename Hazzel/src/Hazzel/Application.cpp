#include "hzpch.h"
#include "Hazzel/Application.h"
#include "Hazzel/Input.h"

#include <glad/glad.h>

namespace Hazzel {

	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGL(ShaderDataType type)
	{
		switch (type)
		{
			case Hazzel::ShaderDataType::Float:		return GL_FLOAT;
			case Hazzel::ShaderDataType::Float2:	return GL_FLOAT;
			case Hazzel::ShaderDataType::Float3:	return GL_FLOAT;
			case Hazzel::ShaderDataType::Float4:	return GL_FLOAT;
			case Hazzel::ShaderDataType::Mat3:		return GL_FLOAT;
			case Hazzel::ShaderDataType::Mat4:		return GL_FLOAT;
			case Hazzel::ShaderDataType::Int:		return GL_INT;
			case Hazzel::ShaderDataType::Int2:		return GL_INT;
			case Hazzel::ShaderDataType::Int3:		return GL_INT;
			case Hazzel::ShaderDataType::Int4:		return GL_INT;
			case Hazzel::ShaderDataType::Bool:		return GL_BOOL;
		}
		HZ_CORE_ASSERT(false, "Unknown ShaderDatayType.");
		return 0;

	}

	Application::Application()
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exist!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));
		
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		glGenVertexArrays(1, &m_VertexArray); 
		glBindVertexArray(m_VertexArray); 
	
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.1f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.8f, 0.1f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.2f, 0.1f, 0.8f, 1.0f,
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
		}; 
		m_VertexBuffer->SetLayout(layout); 

		uint32_t index = 0;
		for (const auto& element : m_VertexBuffer->GetLayout())
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGL(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset
			);
			++index;
		}

		uint32_t indices[3] = { 0, 1, 2 };

		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in  vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0f);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader.reset(Shader::Create(vertexSrc, fragmentSrc));
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1); 
			glClear(GL_COLOR_BUFFER_BIT); 

			m_Shader->Bind();
			glBindVertexArray(m_VertexArray); 
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin(); 
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			//// TESTING: input class
			//auto [x, y] = Input::GetMousePosition();
			//HZ_CORE_TRACE("{0}, {1}", x, y);

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));

		//HZ_CORE_TRACE("{0}", e);	// TESTING: events

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}


	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true; 
	}
}
