#pragma once


#ifdef HZ_PLATFORM_WINDOWS

extern Hazzel::Application* Hazzel::CreateApplication();

int main(int argc, char** argv)
{
	printf("Welcome to Hazzel");
	auto app = Hazzel::CreateApplication();
	app->Run();
	delete app;
	
}

#endif