-- The test project definition
project "devices"
	language    "C++"
	kind        "ConsoleApp"
	uuid        "128cf9bc-944c-4047-b01e-dea2700fc25d"
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

	-- Debug configuration
	filter { "debug" }
		targetdir( "../../bin/" .. action .. "/debug/examples" )
	filter {}

	-- Release configuration
	filter { "release" }
		targetdir( "../../bin/" .. action .. "/release/examples" )
	filter {}
