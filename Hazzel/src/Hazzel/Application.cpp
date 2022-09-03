#include "hzpch.h"
#include "Application.h"

#include "Hazzel/Events/ApplicationEvent.h"
#include "Hazzel/Log.h"

namespace Hazzel {

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			HZ_TRACE(e.ToString());
		}

		while (true);
	}
}
