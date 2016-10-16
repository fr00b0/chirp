-- The _ACTION variable can be null, which will be annoying.
-- Let's make a action that won't be null
action = _ACTION or ""

-- New option to control if the chirp library should be built
-- as a shared or static library
newoption {
	trigger       = "shared",
	description   = "Generate projects for shared library (default: no)",
	value         = "yes/no",
	allowed = {
		{ "yes",   "Shared library" },
		{ "no",    "Static library" }
	}
}

-- The test solution
solution "chirp"
	location             ( "build/" .. action )
	configurations       { "debug", "release" }
	warnings             "Extra"
	flags                { "FatalWarnings" }

	-- Add some flags for gmake builds
	if _ACTION == "gmake" then
		buildoptions     { "-Wall" }
		buildoptions     { "-std=c++11" }
	end

	-- Provide a default for the "shared" option
	if not _OPTIONS["shared"] then
	   _OPTIONS["shared"] = "no"
	end	

	-- Since premake doesn't implement the clean command
	-- on all platforms, we define our own
	if action == "clean" then
		os.rmdir("build")
		os.rmdir("bin")
		os.rmdir("lib")
	end

	-- Debug configuration
	filter { "debug" }
		targetdir        ( "bin/" .. action .. "/debug" )
		defines          { "_DEBUG", "DEBUG" }
		flags            { "Symbols", "Unicode" }
		libdirs          { "lib/" .. action .. "/debug" }
	filter {}

	-- Release configuration
	filter { "release" }
		targetdir        ( "bin/" .. action .. "/release" )
		optimize         ( "Full" )
		defines          { "NDEBUG" }
		flags            { "Unicode" }
		libdirs          { "lib/" .. action .. "/release" }
	filter {}

-- Include the solution projects
include "tests"
include "chirp"
include "examples"
