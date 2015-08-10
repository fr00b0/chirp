#ifndef IG_CHIRP_CHIRP_HPP
#define IG_CHIRP_CHIRP_HPP

#include <chirp/output_devices.hpp>
#include <chirp/audio_format.hpp>
#include <chirp/backend.hpp>

#include <memory>

namespace chirp
{
	/// Enumerator that is used for specifying what implementation
	/// of the audio system that should be used.
	enum class backend_identity {
		/// Use the default implementation for the current platform
		platform_default,
		directsound
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


			/// Create a audio_platform with a already created implementation
			/// @param platform   The platform to use. The ownership of the
			///                   instance is transfered to the new object since
			///                   the unique pointer parameter needs to be moved
			audio_platform( std::unique_ptr<backend::platform> platform );

			///
			///
			///
			output_device default_output_device() const;

		private:
			/// Pointer to the current backend implementation
			std::unique_ptr<backend::platform> _platform_ptr;

	};

}   // namespace chirp

#endif   // IG_CHIRP_CHIRP_HPP