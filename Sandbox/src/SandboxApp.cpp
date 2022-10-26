#include <Hazzel.h>

#include "Platform/OpenGl/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Hazzel::Layer 
{
public:
	ExampleLayer()
		:Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_ModelTransform(glm::mat4(1.0))
	{
		// -----------
		// Triangle
		// -----------
		m_TriangleVertexArray.reset(Hazzel::VertexArray::Create());

		float vertices[3 * 7] = {
			// positions			// color (rgba)
			-0.5f, -0.5f, 0.0f,		0.8f, 0.2f, 0.1f, 1.0f,	// left
			 0.5f, -0.5f, 0.0f,		0.2f, 0.8f, 0.1f, 1.0f, // right
			 0.0f,  0.5f, 0.0f,		0.2f, 0.1f, 0.8f, 1.0f, // center
		};

		Hazzel::Ref<Hazzel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazzel::VertexBuffer::Create(vertices, sizeof(vertices)));
		Hazzel::BufferLayout layout = {
			{Hazzel::ShaderDataType::Float3, "a_Position"},
			{Hazzel::ShaderDataType::Float4, "a_Color"},
		};
		vertexBuffer->SetLayout(layout);
		m_TriangleVertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Hazzel::Ref<Hazzel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazzel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_TriangleVertexArray->SetIndexBuffer(indexBuffer);

		std::string colorSelectVertexSrc = R"(
			#version 330 core

			uniform mat4 u_ModelMatrix;
			uniform mat4 u_ViewProjection;

			layout(location = 0) in  vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			void main()
			{
				gl_Position = u_ViewProjection * u_ModelMatrix * vec4(a_Position, 1.0f);
			}
		)";

		std::string colorSelectFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			uniform vec4 u_InputColor;

			void main()
			{
				color = u_InputColor;
			}
		)";
		m_ColorSelectShader.reset(Hazzel::Shader::Create(colorSelectVertexSrc, colorSelectFragmentSrc));

		// -----------
		// Square
		// -----------
		
		m_SquareVertexArray.reset(Hazzel::VertexArray::Create());

		float squareVertices[4 * 5] = {
			// position				// texture coordinate
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,				// bottom-left
			 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,				// bottom-right
			 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,				// top-right
			-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,				// top-left
		};

		Hazzel::Ref<Hazzel::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(Hazzel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		Hazzel::BufferLayout squareLayout = {
			{Hazzel::ShaderDataType::Float3, "a_Position"},
			{Hazzel::ShaderDataType::Float2, "a_TexCoord"},
		};
		squareVertexBuffer->SetLayout(squareLayout);
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);
	
		uint32_t squareIndices[6] = { 0,1,2,2,3,0 };
		Hazzel::Ref<Hazzel::IndexBuffer> squareIndexBuffer; 
		squareIndexBuffer.reset(Hazzel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		std::string textureVertexShader = R"(
			#version 330 core

			uniform mat4 u_ViewProjection;

			layout(location = 0) in vec3 a_Position; 
			layout(location = 1) in vec2 a_TexCoord; 

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0f);
			} 
		)";

		std::string textureFragmentShader = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";
		m_TextureShader.reset(Hazzel::Shader::Create(textureVertexShader, textureFragmentShader));

		m_CheckerboardTexture = Hazzel::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoTexture = Hazzel::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Hazzel::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Hazzel::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Hazzel::TimeStep ts) 
	{
		//HZ_TRACE("Delta Time: {0} ({1} milliseconds)", ts.GetSeconds(), ts.GetMilliseconds());

		// Input
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

		// Draw
		{
			Hazzel::Renderer::BeginScene(m_Camera);

			// Render flat shaded square matrix
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
			std::dynamic_pointer_cast<Hazzel::OpenGLShader>(m_ColorSelectShader)->Bind();
			std::dynamic_pointer_cast<Hazzel::OpenGLShader>(m_ColorSelectShader)->UploadUniformFloat4("u_InputColor", m_FlateColor);

			for (int i = 0; i < 16; i++)
			{
				for (int j = 0; j < 16; j++)
				{
					m_ModelTransform = glm::translate(glm::mat4(1.0f), glm::vec3((j * 0.11f), (i * 0.11f), 0.0f)) * scale;
					Hazzel::Renderer::Submit(m_ColorSelectShader, m_SquareVertexArray, m_ModelTransform);
				}
			}

			// Render Checkerboard
			m_CheckerboardTexture->Bind(0);
			Hazzel::Renderer::Submit(m_TextureShader, m_SquareVertexArray);

			// Render Cherno Logo
			m_ChernoTexture->Bind(0);
			Hazzel::Renderer::Submit(m_TextureShader, m_SquareVertexArray);

			Hazzel::Renderer::EndScene();
		}
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Test!");
		ImGui::ColorEdit4("Flat Color", glm::value_ptr(m_FlateColor));
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
	Hazzel::Ref<Hazzel::Shader> m_ColorSelectShader, m_TextureShader;
	Hazzel::Ref<Hazzel::VertexArray> m_TriangleVertexArray, m_SquareVertexArray;
	Hazzel::Ref<Hazzel::Texture2D> m_CheckerboardTexture, m_ChernoTexture;

	Hazzel::OrthographicCamera m_Camera;
	
	glm::vec3 m_CameraPosition = { 0.f, 0.f, 0.f };
	const float c_CameraSpeed = 4.0f;

	float m_CameraRotation = 0.f;
	const float c_CameraRotationSpeed = 180.0f;

	glm::mat4 m_ModelTransform;

	glm::vec4 m_FlateColor = { 0.1f, 0.25f, 0.5f, 1.0f };
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