-- premake5.lua
workspace "Hazzel"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug", 
		"Release", 
		"Dist"
	}


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Hazzel/vendor/GLFW/include"
IncludeDir["glad"] = "Hazzel/vendor/glad/include"
IncludeDir["ImGui"] = "Hazzel/vendor/imgui"
IncludeDir["glm"] = "Hazzel/vendor/glm"
IncludeDir["stb_image"] = "Hazzel/vendor/stb_image"


group "Dependencies"
	include "Hazzel/vendor/GLFW"
	include "Hazzel/vendor/glad"
	include "Hazzel/vendor/imgui"
group ""

project "Hazzel"
	location "Hazzel"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hzpch.h"
	pchsource "Hazzel/src/hzpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h", 
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}", 
		"%{IncludeDir.stb_image}", 
	}

	links 
	{ 
		"GLFW",
		"glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines {"HZ_DEBUG", "HZ_ENABLE_ASSERTS"}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_REALEASE"
		runtime "Release"
		optimize "on"
	
	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Hazzel/vendor/spdlog/include",
		"Hazzel/src",
		"Hazzel/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Hazzel"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"HZ_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_REALEASE"
		runtime "Release"
		optimize "on"
	
	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"
