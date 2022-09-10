#pragma once

#include "Hazzel/Renderer/Shader.h"

namespace Hazzel
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertex_source, const std::string& fragment_source);
		virtual ~OpenGLShader(); 

		virtual void Bind() const;
		virtual void Unbind() const;

	private:
		uint32_t m_RendererID; 
	};
}

