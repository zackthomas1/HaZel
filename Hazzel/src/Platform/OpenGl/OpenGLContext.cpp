#include "hzpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <gl/GL.h>

Hazzel::OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
	: m_WindowHandle(windowHandle)
{
	HZ_CORE_ASSERT(windowHandle, "Window handle is null!")
}

void Hazzel::OpenGLContext::Init()
{
	glfwMakeContextCurrent(m_WindowHandle);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	HZ_CORE_ASSERT(status, "Failed to initialize glad!");
}

void Hazzel::OpenGLContext::SwapBuffer()
{
	glfwSwapBuffers(m_WindowHandle);
}
