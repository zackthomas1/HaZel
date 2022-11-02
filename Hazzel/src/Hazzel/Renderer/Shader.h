#pragma once

#include <string>

namespace Hazzel
{
	class Shader
	{
	public:
		virtual ~Shader() {}

		virtual void Bind() const = 0; 
		virtual void Unbind() const = 0; 

		virtual std::string GetName() const = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertex_source, const std::string& fragment_source);
	};

	class ShaderLibrary 
	{
	public:
		void Add(const Ref<Shader>& shader); 
		void Add(const std::string& name, const Ref<Shader>& shader);

		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& vertex_source, const std::string& fragment_source);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const; 
	private:
		std::unordered_map<std::string, Ref<Shader>> m_ShaderMap;
	};
}

