#pragma once

#include "Hazzel/Core/Window.h"
#include "Hazzel/Renderer/GraphicContext.h"

#include <GLFW/glfw3.h>

namespace Hazzel {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props); 
		virtual ~WindowsWindow(); 

		virtual void OnUpdate() override;

		virtual inline unsigned int GetWidth() const override { return m_Data.Width; }
		virtual inline unsigned int GetHeight() const override { return m_Data.Height; }

		virtual inline void* GetNativeWindow() const override { return m_Window; }

		// Window attributes 
		virtual inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSycn() const override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		struct WindowData
		{
			std::string Title; 
			unsigned int Width, Height;
			bool VSync; 

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}


