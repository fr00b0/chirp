#ifndef IG_CHIRP_OUTPUT_DEVICE_HPP
#define IG_CHIRP_OUTPUT_DEVICE_HPP

#include <chirp/audio_format.hpp>
#include <chirp/audio.hpp>
#include <cstdint>

namespace chirp
{
	/// Type used to describe an audio ouput "device"
	///
	/// What a output device actually corresponds to depends on the underlying
	/// implementation, but as far as the chirp library is concerned a
	/// output device is something that can "play" audio.
	///
	class output_device
	{
		public:
			using frequency_type = std::uint32_t;

			audio create_audio( frequency_type frequency, audio_format const& format ) {
				frequency;
				format;
				throw "";
			}

		private:
	};	
}   // namespace chirp

#endif   // IG_CHIRP_OUTPUT_DEVICE_HPP