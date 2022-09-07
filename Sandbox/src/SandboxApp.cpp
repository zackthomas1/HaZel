#include <Hazzel.h>

#include "imgui/imgui.h"

class ExampleLayer : public Hazzel::Layer 
{
public:
	ExampleLayer()
		: Layer("Example") {}

	void OnUpdate() override
	{

		if (Hazzel::Input::IsKeyPressed(HZ_KEY_TAB))
			HZ_TRACE("Tab key is pressed (poll)!");
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Test!");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(Hazzel::Event& event) override
	{
		if (event.GetEventType() == Hazzel::EventType::KeyPressed)
		{
			Hazzel::KeyPressedEvent& e = (Hazzel::KeyPressedEvent&) event;
			
			if(e.GetKeyCode() == HZ_KEY_TAB)
				HZ_TRACE("Tab key is pressed (event)!");
			HZ_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
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