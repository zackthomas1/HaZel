#pragma once

#ifdef HZ_PLATFORM_WINDOWS

extern Hazzel::Application* Hazzel::CreateApplication();

int main(int argc, char** argv)
{
	Hazzel::Log::Init();
	HZ_CORE_WARN("Initialized Log!"); 
	HZ_WARN("Hello World!");

	auto app = Hazzel::CreateApplication();
	app->Run();
	delete app;
}

#endif