#include "hzpch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Hazzel {

	static GLenum ShaderTypeFromString(const std::string& typeStr)
	{
		if (typeStr.compare("vertex") == 0)
		{
			return  GL_VERTEX_SHADER;
		}
		else if (typeStr.compare("fragment") == 0 || typeStr.compare("pixel") == 0)
		{
			return GL_FRAGMENT_SHADER;
		}
		else
		{
			HZ_CORE_ERROR("Invalid shader type declared. '{0}' not recognized", typeStr);
			return 0;
		}
	}

	OpenGLShader::OpenGLShader(const std::string& path)
	{
		std::string source = ReadFile(path); 
		std::unordered_map<GLenum, std::string> shaderSources = PreProcess(source); 
		Compile(shaderSources);
	
		// parse shader name from filepath
		// example filepath string 'assets/shaders/Texture.glsl'
		size_t lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? lastSlash = 0 : lastSlash + 1;

		size_t lastDot = path.rfind('.'); 
		lastDot = lastDot == std::string::npos ? path.length() : lastDot;

		m_Name = path.substr(lastSlash, lastDot - lastSlash);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertex_source, const std::string& fragment_source)
		: m_Name(name)
	{
		std::unordered_map<GLenum, std::string> shaderSources; 
		shaderSources[GL_VERTEX_SHADER] = vertex_source;
		shaderSources[GL_FRAGMENT_SHADER] = fragment_source; 

		Compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	std::string OpenGLShader::GetName() const
	{
		return m_Name;
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, const int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, const float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
	
	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::ifstream in(filepath, std::ios::in | std::ios::binary); 
		std::string result;

		if (in)
		{
			// read file stream into string
			in.seekg(0, in.end);
			result.resize(in.tellg());
			in.seekg(0, in.beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else {
			HZ_CORE_ERROR("Could not open file '{0}'", filepath);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		const char* typeToken = "#type"; 
		size_t typeTokenLength = strlen(typeToken);

		size_t  pos = source.find("#type", 0);
		while (pos != std::string::npos)
		{
			size_t begin = pos + typeTokenLength + 1; 
			size_t eol = source.find_first_of("\r\n", begin);
			HZ_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			std::string shaderType = source.substr(begin, eol - begin);
			HZ_CORE_ASSERT(ShaderTypeFromString(shaderType), "Invalid shader type specified");

			pos = source.find(typeToken, begin);
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			std::string shaderSource = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));

			shaderSources[ShaderTypeFromString(shaderType)] = shaderSource;
			//HZ_CORE_TRACE("source file:\n{0}", shaderSource);
		}
		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();

		HZ_CORE_ASSERT(shaderSources.size() <= 2, "Supports 2 shaders currently.");
		std::array<GLuint, 2> glShaderIDs;
		uint32_t glShaderIDsIndex = 0;

		for (auto& kv : shaderSources) {
			GLenum type = kv.first; 
			const std::string source = kv.second; 

			// Create an empty vertex shader handle
			GLuint shader = glCreateShader(type);

			// Send the shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			// Check that shader was successfully compiled
			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.
				HZ_CORE_ERROR("\t{0}", infoLog.data());
				HZ_CORE_ASSERT(false, "Vertex shader compilation failed.");

				// In this simple program, we'll just leave
				return;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDsIndex++] = shader;
		}

		m_RendererID = program;

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			// Don't leak shaders either.
			for (auto id : glShaderIDs) {
				glDeleteShader(id);
			}

			HZ_CORE_ERROR("\t{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Shader Program linking failed.");

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIDs) {
			glDetachShader(program, id);
		}
	}
}