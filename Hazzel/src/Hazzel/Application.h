#pragma once

#include "Core.h"

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

