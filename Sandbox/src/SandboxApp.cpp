#include <Hazzel.h>

#include "imgui/imgui.h"

class ExampleLayer : public Hazzel::Layer 
{
public:
	ExampleLayer()
		:Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_ModelTransform(glm::mat4(1.0))
	{
		m_VertexArray.reset(Hazzel::VertexArray::Create());

		float vertices[3 * 7] = {
			// positions			// color (rgba)
			-0.5f, -0.5f, 0.0f,		0.8f, 0.2f, 0.1f, 1.0f,	// left
			 0.5f, -0.5f, 0.0f,		0.2f, 0.8f, 0.1f, 1.0f, // right
			 0.0f,  0.5f, 0.0f,		0.2f, 0.1f, 0.8f, 1.0f, // center
		};

		std::shared_ptr<Hazzel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazzel::VertexBuffer::Create(vertices, sizeof(vertices)));
		Hazzel::BufferLayout layout = {
			{Hazzel::ShaderDataType::Float3, "a_Position"},
			{Hazzel::ShaderDataType::Float4, "a_Color"},
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Hazzel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazzel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
			#version 330 core

			uniform mat4 u_ModelMatrix;
			uniform mat4 u_ViewProjection;

			layout(location = 0) in  vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = vec3(u_ModelMatrix * vec4(a_Position, 1.0));
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_ModelMatrix * vec4(a_Position, 1.0f);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			uniform vec4 u_InputColor;

			void main()
			{
				// color = vec4(v_Position * 0.5 + 0.5, 1.0);
				// color = v_Color;
				color = u_InputColor;
			}
		)";
		m_Shader.reset(Hazzel::Shader::Create(vertexSrc, fragmentSrc));
	}

	void OnUpdate(Hazzel::TimeStep ts) 
	{
		//HZ_TRACE("Delta Time: {0} ({1} milliseconds)", ts.GetSeconds(), ts.GetMilliseconds());

		if(Hazzel::Input::IsKeyPressed(HZ_KEY_LEFT))
			m_CameraPosition.x -= c_CameraSpeed * ts;
		else if (Hazzel::Input::IsKeyPressed(HZ_KEY_RIGHT))
			m_CameraPosition.x += c_CameraSpeed * ts;

		if (Hazzel::Input::IsKeyPressed(HZ_KEY_UP))
			m_CameraPosition.y += c_CameraSpeed * ts;
		else if (Hazzel::Input::IsKeyPressed(HZ_KEY_DOWN))
			m_CameraPosition.y -= c_CameraSpeed * ts;

		if (Hazzel::Input::IsKeyPressed(HZ_KEY_A))
			m_CameraRotation -= c_CameraRotationSpeed * ts;
		else if (Hazzel::Input::IsKeyPressed(HZ_KEY_D))
			m_CameraRotation += c_CameraRotationSpeed * ts;

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Hazzel::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		Hazzel::RenderCommand::Clear();

		{
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
			glm::vec4 redColor = { 0.5f, 0.1f, 0.1f, 1.0f };
			glm::vec4 blueColor = { 0.1f, 0.1f, 0.5f, 1.0f };

			Hazzel::Renderer::BeginScene(m_Camera);
			
			for (int i = 0; i < 16; i++)
			{
				for (int j = 0; j < 16; j++)
				{
					m_ModelTransform = glm::translate(glm::mat4(1.0f), glm::vec3((j * 0.1f), (i * 0.1f), 0.0f)) * scale;
					m_Shader->Bind(); 
					m_Shader->UploadUniformVec4fv("u_InputColor", i % 2 == 0 ? redColor : blueColor);
					Hazzel::Renderer::Submit(m_Shader, m_VertexArray, m_ModelTransform);
				}
			}

			Hazzel::Renderer::EndScene();
		}
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Test!");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(Hazzel::Event& event) override
	{
		Hazzel::EventDispatcher dispatcher(event);
		//dispatcher.Dispatch <Hazzel::KeyPressedEvent>(HZ_BIND_EVENT_FN(ExampleLayer::OnKeyPressed)); 
	}

	bool OnKeyPressed(Hazzel::KeyPressedEvent& event)
	{
		switch (event.GetKeyCode())
		{
		case HZ_KEY_LEFT:
			m_CameraPosition.x -= c_CameraSpeed;
			break;
		case HZ_KEY_RIGHT:
			m_CameraPosition.x += c_CameraSpeed;
			break;
		case HZ_KEY_UP:
			m_CameraPosition.y += c_CameraSpeed;
			break;
		case HZ_KEY_DOWN:
			m_CameraPosition.y -= c_CameraSpeed;
			break;
		case HZ_KEY_A:
			m_CameraRotation -= c_CameraRotationSpeed;
			break;
		case HZ_KEY_D:
			m_CameraRotation += c_CameraRotationSpeed;
			break;
		default:
			break;
		}

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
		return false;
	}

private:
	std::shared_ptr<Hazzel::Shader> m_Shader;
	std::shared_ptr<Hazzel::VertexArray> m_VertexArray;

	Hazzel::OrthographicCamera m_Camera;
	
	glm::vec3 m_CameraPosition = { 0.f, 0.f, 0.f };
	const float c_CameraSpeed = 4.0f;

	float m_CameraRotation = 0.f;
	const float c_CameraRotationSpeed = 180.0f;

	glm::mat4 m_ModelTransform;
};

class Sandbox : public Hazzel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer()); 			// TESTING: layers/layerstack and HZ keycodes
	}

	~Sandbox()
	{

	}
};

Hazzel::Application* Hazzel::CreateApplication()
{
	return new Sandbox();
}