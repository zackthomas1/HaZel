#include "hzpch.h"
#include "Shader.h"

#include "Hazzel/Renderer/Renderer.h"
#include "Platform/OpenGl/OpenGLShader.h"

namespace Hazzel
{
	// ------
	// Shader
	// ------
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RenderAPI::None currently not supported.");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(filepath);
		default:
			HZ_CORE_ASSERT(false, "Unknown Renderer API.");
			return nullptr;
		}
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertex_source, const std::string& fragment_source)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RenderAPI::None currently not supported.");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(name, vertex_source, fragment_source);
		default:
			HZ_CORE_ASSERT(false, "Unknown Renderer API.");
			return nullptr;
		}
	}

	// --------------
	// Shader Library
	// --------------
	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		std::string& name = shader->GetName();
		m_ShaderMap[name] = shader;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		HZ_CORE_ASSERT(!Exists(name), "Shader already exist. Existing '{0}' shader overwritten.", name);
		m_ShaderMap[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		Ref<Shader> shader = Shader::Create(filepath);
		Add(shader);

		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& vertex_source, const std::string& fragment_source)
	{
		Ref<Shader> shader = Shader::Create(name, vertex_source, fragment_source);
		Add(name, shader);

		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		HZ_CORE_ASSERT(ShaderLibrary::Exists(name), "Requested Shader does not exist");
		return m_ShaderMap[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_ShaderMap.find(name) != m_ShaderMap.end();
	}

}
