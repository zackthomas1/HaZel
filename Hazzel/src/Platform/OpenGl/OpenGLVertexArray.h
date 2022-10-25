#pragma once
#include "Hazzel/Renderer/VertexArray.h"

namespace Hazzel
{
    class OpenGLVertexArray : public VertexArray
    {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
	private:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers; 
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
    };
}


