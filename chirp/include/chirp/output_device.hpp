#ifndef IG_CHIRP_OUTPUT_DEVICE_HPP
#define IG_CHIRP_OUTPUT_DEVICE_HPP

#include <chirp/audio_format.hpp>
#include <chirp/audio_stream.hpp>
#include <chirp/backend.hpp>
#include <cstdint>
#include <memory>

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
			using frequency_type = backend::output_device::frequency_type;

			/// Default constructor
			output_device() {
			}

			/// Construct form a backend output output_device
			/// @param device   The output device implementation
			output_device( std::shared_ptr<backend::output_device> device ) :
				_device_ptr(device)
			{
			}

			///
			///
			///
			std::string name() const {
				return _device_ptr->name();
			}

			///
			///
			///
			audio_stream create_audio_stream( audio_format const& format ) {
				return audio_stream{ format, _device_ptr->create_audio_stream( format ) };
			}

			///
			///
			///
			bool operator==(output_device const& other) const {
				return _device_ptr != nullptr &&
					   (*_device_ptr) == (*other._device_ptr);
			}

		private:
			std::shared_ptr<backend::output_device> _device_ptr;
	};
}   // namespace chirp

#endif   // IG_CHIRP_OUTPUT_DEVICE_HPP