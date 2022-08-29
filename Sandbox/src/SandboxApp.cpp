#include "Hazzel.h"

class Sandbox : public Hazzel::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}

};

Hazzel::Application* Hazzel::CreateApplication()
{
	return new Sandbox();
}