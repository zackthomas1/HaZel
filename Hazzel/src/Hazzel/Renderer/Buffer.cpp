#include "hzpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGl/OpenGLBuffer.h"

namespace Hazzel
{
	// Vertex buffer
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RenderAPI::None currently not supported.");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		default:
			HZ_CORE_ASSERT(false, "Unknown Renderer API");
			return nullptr;
		}
	}

	// Index buffer
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RenderAPI::None currently not supported.");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLIndexBuffer>(indices, count);
		default:
			HZ_CORE_ASSERT(false, "Unknown Renderer API");
			return nullptr;
		}
	}
}
