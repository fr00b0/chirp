-- Include example projects
include "sinewave"
include "devices"

-- Visual studio builds needs directsound library
filter { "action:vs*" }
	links   { "dsound", "dxguid" }