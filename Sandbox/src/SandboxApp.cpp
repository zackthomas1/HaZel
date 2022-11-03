#include <Hazzel.h>

#include "Platform/OpenGl/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "AppLayers.h"

class Sandbox : public Hazzel::Application
{
public:
	Sandbox()
	{
		PushLayer(new Example3DLayer()); 			
	}

	~Sandbox()
	{

	}
};

Hazzel::Application* Hazzel::CreateApplication()
{
	return new Sandbox();
}