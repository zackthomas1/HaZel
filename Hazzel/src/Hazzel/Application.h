#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Hazzel {
	class HAZZEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	private: 

	};

	// to be defined in CLIENT
	Application* CreateApplication();
}

