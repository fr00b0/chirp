-- Include example projects
include "sinewave"

-- Visual studio builds needs directsound library
filter { "action:vs*" }
	links   { "dsound", "dxguid" }