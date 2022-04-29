workspace "EVB"
	architecture "x64"
	configurations {
		"Release",
		"Debug"
	}

ROOTIncludeDir = "/home/gordon/cern/root-6.22.02/root-install/include"
ROOTLibDir = "/home/gordon/cern/root-6.22.02/root-install/lib"

project "SPSDict"
	kind "SharedLib"
	language "C++"
	cppdialect "c++11"
	targetdir "./lib/"
	objdir "./objs/"

	prebuildcommands {
		"rootcint -f src/spsdict/sps_dict.cxx src/spsdict/DataStructs.h src/spsdict/LinkDef_sps.h",
		"{COPY} src/spsdict/*.pcm ./lib/"
	}

	postbuildcommands {
		"{COPY} src/spsdict/DataStructs.h ./include/"
	}

	files {
		"src/spsdict/DataStructs.h",
		"src/spsdict/*.cpp",
		"src/spsdict/*.cxx"
	}

	includedirs {
		"./",
		"src/spsdict",
	}

	sysincludedirs {
		ROOTIncludeDir
	}

	libdirs {
		ROOTLibDir
	}

	links {
		"Gui", "Core", "Imt", "RIO", "Net", "Hist", 
		"Graf", "Graf3d", "Gpad", "ROOTDataFrame", "ROOTVecOps",
		"Tree", "TreePlayer", "Rint", "Postscript", "Matrix",
		"Physics", "MathCore", "Thread", "MultiProc", "m", "dl"
	}

	filter "system:macosx or linux"
		linkoptions {
			"-pthread",
			"-rdynamic"
		}

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"

project "EventBuilderCore"
	kind "StaticLib"
	language "C++"
	cppdialect "c++11"
	targetdir "./lib/"
	objdir "./objs/"
	pchheader "EventBuilder.h"
	pchsource "./src/EventBuilder.cpp"

	files {
		"src/spsdict/DataStructs.h",
		"src/evb/*.cpp",
		"src/evb/*.h"
	}

	defines "ETC_DIR_PATH=\"./etc/\""

	includedirs {
		"./",
		"src/",
		"vendor/spdlog/include",
		"src/evb",
		"src/spsdict",
		"src/guidict"
	}

	sysincludedirs {
		ROOTIncludeDir
	}

	libdirs {
		ROOTLibDir,
	}

	links {
		"SPSDict", "Gui", "Core", "Imt", "RIO", "Net", "Hist", 
		"Graf", "Graf3d", "Gpad", "ROOTDataFrame", "ROOTVecOps",
		"Tree", "TreePlayer", "Rint", "Postscript", "Matrix",
		"Physics", "MathCore", "Thread", "MultiProc", "m", "dl"
	}

	filter "system:macosx or linux"
		linkoptions {
			"-pthread",
			"-rdynamic"
		}

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"

project "EventBuilderGui"
	kind "ConsoleApp"
	language "C++"
	cppdialect "c++11"
	targetdir "./bin/"
	objdir "./objs/"

	prebuildcommands {
		"rootcint -f src/guidict/gui_dict.cxx src/guidict/EVBMainFrame.h src/guidict/FileViewFrame.h src/guidict/LinkDef_Gui.h",
		"{COPY} src/guidict/*.pcm ./bin/"
	}

	files {
		"src/guidict/FileViewFrame.h",
		"src/guidict/EVBMainFrame.h",
		"src/guidict/*.cpp",
		"src/guidict/gui_dict.cxx",
		"src/gui_main.cpp"
	}

	includedirs {
		"./",
		"vendor/spdlog/include",
		"src/evb",
		"src/spsdict",
		"src/guidict"
	}

	sysincludedirs {
		ROOTIncludeDir
	}

	libdirs {
		ROOTLibDir,
	}

	links {
		"EventBuilderCore", "SPSDict", "Gui", "Core", "Imt", "RIO", "Net", "Hist", 
		"Graf", "Graf3d", "Gpad", "ROOTDataFrame", "ROOTVecOps",
		"Tree", "TreePlayer", "Rint", "Postscript", "Matrix",
		"Physics", "MathCore", "Thread", "MultiProc", "m", "dl"
	}

	filter "system:macosx or linux"
		linkoptions {
			"-pthread",
			"-rdynamic"
		}

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"

project "EventBuilder"
	kind "ConsoleApp"
	language "C++"
	cppdialect "c++11"
	targetdir "./bin/"
	objdir "./objs/"

	files {
		"src/main.cpp"
	}

	includedirs {
		"src/",
		"vendor/spdlog/include",
		"src/evb",
		"src/spsdict",
		"src/guidict"
	}

	sysincludedirs {
		ROOTIncludeDir
	}

	libdirs {
		ROOTLibDir,
	}

	links {
		"EventBuilderCore", "SPSDict", "Gui", "Core", "Imt", "RIO", "Net", "Hist", 
		"Graf", "Graf3d", "Gpad", "ROOTDataFrame", "ROOTVecOps",
		"Tree", "TreePlayer", "Rint", "Postscript", "Matrix",
		"Physics", "MathCore", "Thread", "MultiProc", "m", "dl"
	}

	filter "system:macosx or linux"
		linkoptions {
			"-pthread",
			"-rdynamic"
		}

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"