#ifndef IG_CHIRP_STATIC_CONFIG_HPP
#define IG_CHIRP_STATIC_CONFIG_HPP

#if defined(_WINVER)
#else
	// If we are not on windows, we disable DirectSound support
#	undef CHIRP_WITH_DIRECTSOUND
#endif

#endif