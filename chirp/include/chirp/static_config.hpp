#ifndef IG_CHIRP_STATIC_CONFIG_HPP
#define IG_CHIRP_STATIC_CONFIG_HPP

#if _WIN32 && !defined(CHIRP_WITHOUT_DIRECTSOUND)
#	define CHIRP_WITH_DIRECTSOUND
#else
	// If we are not on windows or have disabled direct sound support
#	undef CHIRP_WITH_DIRECTSOUND
#endif

#endif