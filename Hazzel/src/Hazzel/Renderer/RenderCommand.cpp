#include "hzpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGl/OpenGLRendererAPI.h"

namespace Hazzel {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}