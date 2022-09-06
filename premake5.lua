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

group "Dependencies"
	include "Hazzel/vendor/GLFW"
	include "Hazzel/vendor/glad"
	include "Hazzel/vendor/imgui"
group ""

project "Hazzel"
	location "Hazzel"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hzpch.h"
	pchsource "Hazzel/src/hzpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links 
	{ 
		"GLFW",
		"glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands 
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines {"HZ_DEBUG", "HZ_ENABLE_ASSERTS"}
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "HZ_REALEASE"
		runtime "Release"
		optimize "On"
	
	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

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
		"%{IncludeDir.glm}"
	}

	links
	{
		"Hazzel"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"HZ_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "HZ_REALEASE"
		runtime "Release"
		optimize "On"
	
	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "On"

