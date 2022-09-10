#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Hazzel{

	class HAZZEL_API Log
	{
	public:
		static void Init(); 

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()		{ return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger()	{ return s_ClientLogger;  }
	
	private: 
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define HZ_CORE_TRACE(...)		::Hazzel::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HZ_CORE_INFO(...)		::Hazzel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HZ_CORE_WARN(...)		::Hazzel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HZ_CORE_ERROR(...)		::Hazzel::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HZ_CORE_FATAL(...)		::Hazzel::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define HZ_TRACE(...)			::Hazzel::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HZ_INFO(...)			::Hazzel::Log::GetClientLogger()->info(__VA_ARGS__)
#define HZ_WARN(...)			::Hazzel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HZ_ERROR(...)			::Hazzel::Log::GetClientLogger()->error(__VA_ARGS__)
#define HZ_FATAL(...)			::Hazzel::Log::GetClientLogger()->critical(__VA_ARGS__)