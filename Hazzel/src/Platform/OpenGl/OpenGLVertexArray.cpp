#include "hzpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Hazzel
{
	static GLenum ShaderDataTypeToOpenGL(ShaderDataType type)
	{
		switch (type)
		{
		case Hazzel::ShaderDataType::Float:		return GL_FLOAT;
		case Hazzel::ShaderDataType::Float2:	return GL_FLOAT;
		case Hazzel::ShaderDataType::Float3:	return GL_FLOAT;
		case Hazzel::ShaderDataType::Float4:	return GL_FLOAT;
		case Hazzel::ShaderDataType::Mat3:		return GL_FLOAT;
		case Hazzel::ShaderDataType::Mat4:		return GL_FLOAT;
		case Hazzel::ShaderDataType::Int:		return GL_INT;
		case Hazzel::ShaderDataType::Int2:		return GL_INT;
		case Hazzel::ShaderDataType::Int3:		return GL_INT;
		case Hazzel::ShaderDataType::Int4:		return GL_INT;
		case Hazzel::ShaderDataType::Bool:		return GL_BOOL;
		}
		HZ_CORE_ASSERT(false, "Unknown ShaderDatayType.");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		HZ_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no Layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(m_VertexBufferIndex);
			glVertexAttribPointer(m_VertexBufferIndex,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGL(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset
			);
			++m_VertexBufferIndex;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID); 
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}
