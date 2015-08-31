#ifndef IG_CHIRP_SAMPLE_REQUEST_HPP
#define IG_CHIRP_SAMPLE_REQUEST_HPP

#include <chirp/audio_format.hpp>

#include <chrono>
#include <cstdint>

namespace chirp
{
	/// duration type
	using duration_type = std::chrono::duration<float>;

	/// A request for samples to fill a target buffer.
	class sample_request
	{
		public:
			/// pointer type to the target buffer
			using pointer = void*;
			/// duration type
			using duration_type = chirp::duration_type;
			/// integral type for byte counts
			using byte_count = audio_format::byte_count;
			/// integral type for sample counts
			using sample_count = audio_format::byte_count;

			/// Create a sample_request
			///
			/// @param buffer_ptr    Pointer to the target buffer
			/// @param buffer_size   Size of the target buffer, in bytes
			/// @param format        The audio format that is requested
			sample_request( pointer buffer_ptr, byte_count buffer_size, audio_format const& format ) :
				_start_ptr( buffer_ptr ),
				_end_ptr( static_cast<std::uint8_t*>(buffer_ptr) + buffer_size ),
				_format( format )
			{}

			/// @returns Pointer to the start of the buffer that is the target
			///          of the sample request. Writing data to bytes before
			///          this point while fullfilling the request results
			///          in undefined behaviour.
			pointer buffer_start() const {
				return _start_ptr;
			}

			/// @returns Pointer to the end of the buffer that is the target
			///          of the sample request. Writing data to bytes after
			///          this point while fullfilling the request results
			///          in undefined behaviour.
			pointer buffer_end() const {
				return _end_ptr;
			}

			/// @returns The size (in bytes) of the buffer that is the target
			///          ofg the sample request.
			byte_count buffer_size() const {
				return static_cast<byte_count>(
					static_cast<std::uint8_t const*>(_end_ptr) -
					static_cast<std::uint8_t const*>(_start_ptr) );
			}

			/// @return The audio format that is requested.
			audio_format const& format() const {
				return _format.get();
			}

			/// @returns The number of frames of samples that is requested to
			///          be written to the target buffer.
			sample_count frames() const {
				return buffer_size() / _format.get().bytes_per_frame();
			}

			/// @returns The duration of the requested data, with the audio
			///          of the request.
			duration_type duration() const {
				return duration_type{ frames() / (float)_format.get().frequency() };
			}

		private:
			/// Start of the target buffer
			pointer _start_ptr;
			/// End of the target buffer
			pointer _end_ptr;
			/// Format of the audio data that i requested
			std::reference_wrapper<audio_format const> _format;
	};
}

#endif