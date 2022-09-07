#pragma once

#include "Hazzel/Renderer/GraphicContext.h"

struct GLFWwindow;

namespace Hazzel 
{
	class OpenGLContext : public GraphicsContext
	{
	public:

		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffer() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}


