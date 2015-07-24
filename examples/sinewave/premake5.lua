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
	-- Debug configuration
	filter { "debug" }
		targetdir        ( "../../bin/" .. action .. "/debug/examples" )

	-- Release configuration
	filter { "release" }
		targetdir        ( "../../bin/" .. action .. "/release/examples" )
