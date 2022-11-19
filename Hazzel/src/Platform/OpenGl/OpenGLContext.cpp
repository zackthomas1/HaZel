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

	HZ_CORE_INFO("OpenGL Info: "); 
	HZ_CORE_INFO("\t Vendor: {0}", glGetString(GL_VENDOR));
	HZ_CORE_INFO("\t Render: {0}", glGetString(GL_RENDERER));
	HZ_CORE_INFO("\t Version: {0}", glGetString(GL_VERSION));
}

void Hazzel::OpenGLContext::SwapBuffer()
{
	glfwSwapBuffers(m_WindowHandle);
}
