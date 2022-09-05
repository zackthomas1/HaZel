#pragma once

#include "hzpch.h"

#include "Hazzel/Core.h"
#include "Hazzel/Events/Event.h"

namespace Hazzel {
	
	struct WindowProps
	{
		std::string Title; 
		unsigned int Width, Height; 

		WindowProps(const std::string& title = "Hazzel Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height) {}
	};

	// Interface reprensenting a desktop system based Window
	class HAZZEL_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0; 

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void* GetNativeWindow() const = 0;

		// Window attributes 
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSycn() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}