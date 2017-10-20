-- The test project definition
project "chirp_tests"
	language      "C++"
	kind          "ConsoleApp"
	uuid          "0e85239e-24f0-43ce-8507-363507b2b166"
	includedirs   { ".", "../chirp/include" }
	links         { "chirp" }
	files {
		"**.hpp",
		"**.cpp"
	}

	-- Visual studio builds needs directsound library
	filter { "action:vs*" }
		links     { "dsound", "dxguid" }
	filter {}

	-- Linux builds needs alsa
	filter { "action:gmake" }
		links	{ "asound" }
	filter {}
