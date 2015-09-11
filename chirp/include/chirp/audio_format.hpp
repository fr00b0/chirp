#ifndef IG_CHIRP_AUDIO_FORMAT_HPP
#define IG_CHIRP_AUDIO_FORMAT_HPP

#include <chirp/exceptions.hpp>
#include <chirp/sample_format.hpp>
#include <chrono>
#include <cstdint>

namespace chirp
{
	/// duration type
	using duration_type = std::chrono::duration<float>;

	///
	///
	///
	class audio_format
	{
		public:
			///
			using frequency_type = std::uint32_t;
			using bit_count = chirp::sample_format::bit_count;
			using byte_count = chirp::sample_format::byte_count;
			using channel_count = chirp::sample_format::channel_count;

			///
			audio_format( frequency_type frequency, sample_format const& format ) :
				_frequency(frequency),
				_sample_format(format),
				_bytes_per_second( format.bytes_per_frame() * _frequency),
				_duration_per_frame( std::chrono::duration_cast<duration_type>( std::chrono::duration<float>( 1.0f / frequency ) ) )
			{
			}

			///
			frequency_type frequency() const {
				return _frequency;
			}

			///
			chirp::sample_format const& sample_format() const {
				return _sample_format;
			}

			///
			bit_count bits_per_sample() const {
				return _sample_format.bits_per_sample();
			}

			///
			channel_count channels() const {
				return _sample_format.channels();
			}

			///
			byte_count bytes_per_frame() const {
				return _sample_format.bytes_per_frame();
			}

			///
			byte_order endianness() const {
				return _sample_format.endianness();
			}

			///
			byte_count bytes_per_second() const {
				return _bytes_per_second;
			}

			///
			duration_type duration_per_frame() const {
				return _duration_per_frame;
			}

			///
			bool operator==( audio_format const& other ) const {
				return _frequency == other._frequency &&
				       _sample_format == other._sample_format;
			}

			///
			bool operator!=( audio_format const& other ) const {
				return !(*this == other);
			}

		private:
			/// The audio formats frequency
			frequency_type _frequency;
			/// The audio formats sample format
			chirp::sample_format _sample_format;
			/// Number of bytes per second
			byte_count _bytes_per_second;
			/// The audio formats time duration per frame
			chirp::duration_type _duration_per_frame;
	};
}   // namespace chirp

#endif   // IG_CHIRP_AUDIO_FORMAT_HPP