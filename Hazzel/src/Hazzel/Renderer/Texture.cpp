#include "hzpch.h"
#include "Renderer.h"
#include "Hazzel/Core/Core.h"
#include "Texture.h"

#include "Platform/OpenGl/OpenGLTexture.h"

namespace Hazzel {
	Ref<Texture2D> Texture2D::Create(std::string path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RenderAPI::None currently not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(path);
		default:
			HZ_CORE_ASSERT(false, "Unknown Renderer API");
			return nullptr;
		}
	}
}

