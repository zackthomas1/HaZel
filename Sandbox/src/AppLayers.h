#pragma once
#include <Hazzel.h>

#include "Platform/OpenGl/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// ----------------
// Example Layer
// ----------------

class ExampleLayer : public Hazzel::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_ModelTransform(glm::mat4(1.0))
	{
		// Triangle
		// -----------
		m_TriangleVertexArray = Hazzel::VertexArray::Create();

		float triangleVertices[3 * 7] = {
			// positions			// color (rgba)
			-0.5f, -0.5f, 0.0f,		0.8f, 0.2f, 0.1f, 1.0f,	// left
			 0.5f, -0.5f, 0.0f,		0.2f, 0.8f, 0.1f, 1.0f, // right
			 0.0f,  0.5f, 0.0f,		0.2f, 0.1f, 0.8f, 1.0f, // center
		};

		Hazzel::Ref<Hazzel::VertexBuffer> triangleVertexBuffer = Hazzel::VertexBuffer::Create(triangleVertices, sizeof(triangleVertices));
		Hazzel::BufferLayout triangleLayout = {
			{Hazzel::ShaderDataType::Float3, "a_Position"},
			{Hazzel::ShaderDataType::Float4, "a_Color"},
		};
		triangleVertexBuffer->SetLayout(triangleLayout);
		m_TriangleVertexArray->AddVertexBuffer(triangleVertexBuffer);

		uint32_t triangleIndices[3] = { 0, 1, 2 };
		Hazzel::Ref<Hazzel::IndexBuffer> triangleIndexBuffer = Hazzel::IndexBuffer::Create(triangleIndices, sizeof(triangleIndices) / sizeof(uint32_t));
		m_TriangleVertexArray->SetIndexBuffer(triangleIndexBuffer);

		// Square
		// -----------
		m_SquareVertexArray = Hazzel::VertexArray::Create();

		float squareVertices[4 * 5] = {
			// position				// texture coordinate
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,				// bottom-left
			 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,				// bottom-right
			 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,				// top-right
			-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,				// top-left
		};

		Hazzel::Ref<Hazzel::VertexBuffer> squareVertexBuffer = Hazzel::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		Hazzel::BufferLayout squareLayout = {
			{Hazzel::ShaderDataType::Float3, "a_Position"},
			{Hazzel::ShaderDataType::Float2, "a_TexCoord"},
		};
		squareVertexBuffer->SetLayout(squareLayout);
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

		uint32_t squareIndices[6] = { 0,1,2,2,3,0 };
		Hazzel::Ref<Hazzel::IndexBuffer> squareIndexBuffer = Hazzel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		// Shaders 
		// -----------
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
		m_ColorSelectShader = Hazzel::Shader::Create("Color Select", colorSelectVertexSrc, colorSelectFragmentSrc);

		Hazzel::Ref<Hazzel::Shader> textureShader = m_ShaderLib.Load("assets/shaders/Texture.glsl");

		m_CheckerboardTexture = Hazzel::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoTexture = Hazzel::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Hazzel::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Hazzel::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Hazzel::TimeStep ts)
	{
		//HZ_TRACE("Delta Time: {0} ({1} milliseconds)", ts.GetSeconds(), ts.GetMilliseconds());

		// Input
		if (Hazzel::Input::IsKeyPressed(HZ_KEY_LEFT))
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

		// Draw
		Hazzel::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		Hazzel::RenderCommand::Clear();

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
			Hazzel::Ref<Hazzel::Shader> textureShader = m_ShaderLib.Get("Texture");

			m_CheckerboardTexture->Bind(0);
			Hazzel::Renderer::Submit(textureShader, m_SquareVertexArray);

			// Render Cherno Logo
			m_ChernoTexture->Bind(0);
			Hazzel::Renderer::Submit(textureShader, m_SquareVertexArray);

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
	Hazzel::ShaderLibrary m_ShaderLib;
	Hazzel::Ref<Hazzel::Shader> m_ColorSelectShader;
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


// ----------------
// Example 3D Layer
// ----------------

class Example3DLayer : public Hazzel::Layer
{
public:

	Example3DLayer()
		:Layer("Example 3D"), m_Camera(1280.0 / 720.0f, 45.0f, 0.1, 100.f)
	{
		m_SquareVA = Hazzel::VertexArray::Create();
		float squareVertices[4 * 5] = {
			// position				// texture coordinate
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,				// bottom-left
			 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,				// bottom-right
			 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,				// top-right
			-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,				// top-left
		};

		Hazzel::Ref<Hazzel::VertexBuffer> squareVB = Hazzel::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		Hazzel::BufferLayout squareLayout = {
			{Hazzel::ShaderDataType::Float3, "position" }, 
			{Hazzel::ShaderDataType::Float2, "textureCoord"}
		};
		squareVB->SetLayout(squareLayout); 
		m_SquareVA->AddVertexBuffer(squareVB); 

		uint32_t squareIndices[6] = { 0,1,2,2,3,0 };
		Hazzel::Ref<Hazzel::IndexBuffer> squareIB = Hazzel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(squareIB); 

		Hazzel::Ref<Hazzel::Shader> squareShader = m_ShaderLib.Load("assets/shaders/Square.glsl"); 
		m_BrickWallTexture = Hazzel::Texture2D::Create("assets/textures/wall.jpg");
		
		std::dynamic_pointer_cast<Hazzel::OpenGLShader>(squareShader)->Bind(); 
		std::dynamic_pointer_cast<Hazzel::OpenGLShader>(squareShader)->UploadUniformInt("u_Texture", 0);
	}

	virtual void OnAttach() {}

	virtual void OnUpdate(Hazzel::TimeStep ts) 
	{

		// Update
		glm::vec3 camPos = m_Camera.GetPosition();
		if (Hazzel::Input::IsKeyPressed(HZ_KEY_W))
		{
			m_Camera.SetPosition(camPos += (m_CameraSpeed * ts) * m_Camera.GetLookDirection());
		}
		else if (Hazzel::Input::IsKeyPressed(HZ_KEY_S))
		{
			m_Camera.SetPosition(camPos -= (m_CameraSpeed * ts) * m_Camera.GetLookDirection());
		}
		else if (Hazzel::Input::IsKeyPressed(HZ_KEY_D))
		{
			m_Camera.SetPosition(camPos += (m_CameraSpeed * ts)
				* glm::normalize(glm::cross(m_Camera.GetLookDirection(), m_Camera.GetUpDirection()))
			);
		}
		else if (Hazzel::Input::IsKeyPressed(HZ_KEY_A))
		{
			m_Camera.SetPosition(camPos -= (m_CameraSpeed * ts)
				* glm::normalize(glm::cross(m_Camera.GetLookDirection(), m_Camera.GetUpDirection()))
			);
		}

		glm::mat4 modelTransform = glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		//Render
		Hazzel::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		Hazzel::RenderCommand::Clear();

		Hazzel::Ref<Hazzel::Shader> squareShader = m_ShaderLib.Get("Square");
		{
			Hazzel::Renderer::BeginScene(m_Camera);

			m_BrickWallTexture->Bind(0);
			Hazzel::Renderer::Submit(squareShader, m_SquareVA, modelTransform);

			Hazzel::Renderer::EndScene();
		}
}

	virtual void OnImGuiRender() 
	{
		ImGui::Begin("Example 3D");

		ImGui::End();
	}

	virtual void OnEvent(Hazzel::Event& event) 
	{
		Hazzel::EventDispatcher dispatcher(event);
		dispatcher.Dispatch <Hazzel::MouseMovedEvent>(HZ_BIND_EVENT_FN(Example3DLayer::OnMouseMoved));
		dispatcher.Dispatch <Hazzel::MouseScrolledEvent>(HZ_BIND_EVENT_FN(Example3DLayer::OnMouseScrolled)); 
	}

	bool OnMouseMoved(Hazzel::MouseMovedEvent& event)
	{
		// TODO: Check if the mouse has left or entered window
		if (m_FirstMouse)
		{
			m_MouseX = event.GetX(); 
			m_MouseY = event.GetY();
			m_FirstMouse = false;
		}

		float xOffset = event.GetX() - m_MouseX; 
		float yOffset = m_MouseY - event.GetY();
		m_MouseX = event.GetX();
		m_MouseY = event.GetY();
	
		xOffset *= m_MouseSensitivity;
		yOffset *= m_MouseSensitivity;

		m_Camera.SetYaw(m_Camera.GetYaw() + xOffset);
		m_Camera.SetPitch(m_Camera.GetPitch() + yOffset);

		return true;
	}

	bool OnMouseScrolled(Hazzel::MouseScrolledEvent& event)
	{
		m_Camera.SetFOV(m_Camera.GetFOV() - event.GetYOffset());

		return true;
	}

private:
	Hazzel::ShaderLibrary m_ShaderLib;
	Hazzel::Ref<Hazzel::Texture2D> m_BrickWallTexture;

	Hazzel::Ref<Hazzel::VertexArray> m_SquareVA;
	Hazzel::PerspectiveCamera m_Camera;

	float m_MouseX = 1280.f / 2.f, m_MouseY = 720.f / 2.f;

	const float m_CameraSpeed = 5.0f;
	const float m_MouseSensitivity = 0.25f;
	
	bool m_FirstMouse = true;
};