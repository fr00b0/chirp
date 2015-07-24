-- The test project definition
project "chirp"
	language    "C++"
	uuid        "11fbb9e1-bc9e-4f73-82a4-ab13c5130204"
	includedirs { "include" }	
	files {
		"include/**.hpp",
		"src/**.cpp" 		
	}

	-- Debug configuration, static library
	filter { "debug", "options:shared=no" }
		kind         "StaticLib"
		targetdir   ( "../lib/" .. action .. "/debug" )

	-- Release configuration, static library
	filter { "release", "options:shared=no" }
		kind         "StaticLib"
		targetdir   ( "../lib/" .. action .. "/release" )

	-- Debug configuration, shared library
	filter { "debug", "options:shared=yes" }
		kind         "SharedLib"
		targetdir   ( "../bin/" .. action .. "/debug" )

	-- Release configuration, shared library
	filter { "release", "options:shared=yes" }
		kind         "SharedLib"
		targetdir   ( "../bin/" .. action .. "/release" )

	-- Visual studio builds will include DirectSound
	filter { "vs*" }
		defines { "CHIRP_WITH_DIRECTSOUND" }
