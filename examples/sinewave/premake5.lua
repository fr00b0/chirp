-- The test project definition
project "sinewave"
	language    "C++"
	kind        "ConsoleApp"
	uuid        "c2edf458-fd5a-4c47-b649-c0e799183eea"
	includedirs { ".", "../../chirp/include" }
	links       { "chirp" }
	files {
		"**.hpp",
		"**.cpp"
	}

	-- Visual studio builds needs directsound library
	filter { "action:vs*" }
		links   { "dsound", "dxguid" }
	filter {}

	-- Linux builds needs alsa
	filter { "action:gmake" }
		links	{ "asound" }
	filter {}


	-- Debug configuration
	filter { "debug" }
		targetdir( "../../bin/" .. action .. "/debug/examples" )
	filter {}

	-- Release configuration
	filter { "release" }
		targetdir( "../../bin/" .. action .. "/release/examples" )
	filter {}
