#include "Hazzel.h"

class ExampleLayer : public Hazzel::Layer 
{
public:
	ExampleLayer()
		: Layer("Example") {}

	void OnUpdate() override
	{
		HZ_INFO("ExampleLayer::Update");
	}

	void OnEvent(Hazzel::Event& event) override
	{
		HZ_TRACE("{0}", event);
	}
};

class Sandbox : public Hazzel::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer()); 			// TESTING: layers and layerstack
		PushLayer(new Hazzel::ImGuiLayer());
	}

	~Sandbox()
	{

	}

};

Hazzel::Application* Hazzel::CreateApplication()
{
	return new Sandbox();

}