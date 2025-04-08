workspace "OpenGL-3D-ScratchPad"
	configurations {
		"Debug", "Release"
	}
	platforms {
		"Win32", "Win64"
	}

	filter {"platforms:Win32"}
		system "Windows"
		architecture "x86"

	filter {"platforms:Win64"}
		system "Windows"
		architecture "x86_64"


project "OpenGL-3D"
	kind "ConsoleApp"
	language "C++"

	location "OpenGL-3D"

	files {
		"OpenGL-3D/src/**.h",
		"OpenGL-3D/src/**.cpp",
		"OpenGL-3D/src/**.c",

		"OpenGL-3D/src/shaders/*.glsl",
	}
	includedirs {
		"Dependencies/GLAD/include",
		"Dependencies/GLFW/include",
		"Dependencies/glm"	
	}
	libdirs {
		"Dependencies/GLFW/lib-vc2022",
	}
	links {
		"opengl32",
		"glfw3"
	}

	targetdir "bin/%{prj.name}/%{cfg.buildcfg}-%{cfg.platform}/"
