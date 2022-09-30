#include "hzpch.h"
#include "Shader.h"

#include "Hazzel/Renderer/Renderer.h"
#include "Platform/OpenGl/OpenGLShader.h"

namespace Hazzel
{
	Shader* Shader::Create(const std::string& vertex_source, const std::string& fragment_source)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RenderAPI::None currently not supported.");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertex_source, fragment_source);
		default:
			HZ_CORE_ASSERT(false, "Unknown Renderer API.");
			return nullptr;
		}
	}
}
