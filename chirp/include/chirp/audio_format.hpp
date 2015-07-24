#ifndef IG_CHIRP_AUDIO_FORMAT_HPP
#define IG_CHIRP_AUDIO_FORMAT_HPP

#include <chirp/exceptions.hpp>
#include <cstdint>

namespace chirp
{
	///
	///
	///
	enum class byte_order {
		big_endian,
		little_endian
	};

	///
	///
	///
	class audio_format
	{
		public:
			// Not default constructible
			audio_format() = delete;

			// Typedefs
			/// Integral type for counting channels (unsigned 8 bit integer)
			using channel_count = std::uint8_t;
			/// Integral type for counting bits (unsigned 8 bit integer)
			using bit_count = std::uint8_t;

			/// Construct an audio format with a maximum of eight bits per sample.
			///
			/// @param bits_per_sample   The number of bits that each sample
			///                          consists of.
			/// @param channels          The number of interleaved channels
			/// @pre bits_per_sample <= 8
			audio_format( bit_count bits_per_sample, channel_count channels ) :
				_bits_per_sample( bits_per_sample ),
				_endianness( byte_order::little_endian ),
				_channels( channels )
			{
				if( _bits_per_sample > 8 ) {
					throw byte_order_exception{};
				}
			}

			/// Construct an audio format, with more than eight bits per sample.
			///
			/// @param bits_per_sample   The number of bits that each sample
			///                          consists of. This value must be >8
			/// @param endianness        The byte order of each sample
			/// @param channels          The number of interleaved channels
			/// @pre bits_per_sample > 8
			audio_format( bit_count bits_per_sample, byte_order endianness, channel_count channels ) :
				_bits_per_sample( bits_per_sample ),
				_endianness( endianness ),
				_channels( channels )
			{
				if( _bits_per_sample <= 8 ) {
					throw byte_order_exception{};
				}
			}

			/// Retrieve the bits per sample of the audio format
			/// @returns The number of bits per individual sample
			bit_count bits_per_sample() const {
				return _bits_per_sample;
			}

			/// Retrieve the endianness of the audio format
			/// @returns The byte order of each individual sample
			byte_order endianness() const {
				if( _bits_per_sample <=8 ) {
					throw byte_order_exception{};
				}
				return _endianness;
			}

			/// Retrieve the number channels of the audio format
			/// @returns The number of interleaved channels
			channel_count channels() const {
				return _channels;
			}

			///
			bool operator==( audio_format const& lhs ) const {
				return _bits_per_sample == lhs._bits_per_sample &&
				       _endianness == lhs._endianness && 
				       _channels == lhs._channels;
			}

			///
			bool operator!=( audio_format const& lhs ) const {
				return !(*this == lhs);
			}

		private:
			bit_count _bits_per_sample;   ///< The number of bits per individual sample
			byte_order _endianness;       ///< The byte order of each sample
			channel_count _channels;      ///< The number of interleaved channels
	};



	// Constants for common audio formats
	extern audio_format const eight_bit_mono;
	extern audio_format const eight_bit_stereo;
	extern audio_format const sixteen_bits_little_endian_mono;
	extern audio_format const sixteen_bits_big_endian_mono;
	extern audio_format const sixteen_bits_little_endian_stereo;
	extern audio_format const sixteen_bits_big_endian_stereo;


}   // namespace chirp

#endif   // IG_CHIRP_AUDIO_FORMAT_HPP