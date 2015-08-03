#ifndef IG_CHIRP_CHIRP_HPP
#define IG_CHIRP_CHIRP_HPP

#include <chirp/output_devices.hpp>
#include <chirp/audio_format.hpp>

namespace chirp
{
	/// Enumerator that is used for specifying what implementation
	/// of the audio system that should be used.
	enum class backend_identity {
		/// Use the default implementation for the current platform
		platform_default,
		direct_sound
	};

	/// Audio platform
	///
	/// This is a wrapper class for the audio platform implementation
	/// that is at the heart of the chirp library.
	class audio_platform
	{
		public:
			/// Create a audio_platform with a requested implementation
			/// @throws unsupported_implementation when the requested implementation
			/// couldn't be created.
			audio_platform( backend_identity request = backend_identity::platform_default );

			///
			///
			///
			output_device default_output_device() const;

		private:

	};

}   // namespace chirp

#endif   // IG_CHIRP_CHIRP_HPP