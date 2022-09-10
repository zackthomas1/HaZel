#include "hzpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGl/OpenGLBuffer.h"

namespace Hazzel
{
	// Vertex buffer
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::None:
			HZ_CORE_ASSERT(false, "RenderAPI::None currently not supported.");
			return nullptr;
		case RenderAPI::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		default:
			HZ_CORE_ASSERT(false, "Unknown Render API");
			return nullptr;
		}
	}

	// Index buffer
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::None:
			HZ_CORE_ASSERT(false, "RenderAPI::None currently not supported.");
			return nullptr;
		case RenderAPI::OpenGL:
			return new OpenGLIndexBuffer(indices, count);
		default:
			HZ_CORE_ASSERT(false, "Unknown Render API");
			return nullptr;
		}
	}
}
