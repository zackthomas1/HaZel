#pragma once

#include "Hazzel/Renderer/Shader.h"

namespace Hazzel
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertex_source, const std::string& fragment_source);
		virtual ~OpenGLShader(); 

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) override;
		virtual void UploadUniformVec4fv(const std::string& name, const glm::vec4& vec) override;

	private:
		uint32_t m_RendererID; 
	};
}

