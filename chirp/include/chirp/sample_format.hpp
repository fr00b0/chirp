#ifndef IG_CHIRP_SAMPLE_FORMAT_HPP
#define IG_CHIRP_SAMPLE_FORMAT_HPP

#include <chirp/exceptions.hpp>

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
	class sample_format
	{
		public:
			// Not default constructible
			sample_format() = delete;

			// Typedefs
			/// Integral type for counting channels (unsigned 8 bit integer)
			using channel_count = std::uint8_t;
			/// Integral type for counting bits (unsigned 8 bit integer)
			using bit_count = std::uint8_t;
			/// Integral type for byte sizes
			using byte_count = std::uint32_t;

			/// Construct an sample format with a maximum of eight bits per sample.
			///
			/// @param bits_per_sample   The number of bits that each sample
			///                          consists of.
			/// @param channels          The number of interleaved channels
			/// @pre bits_per_sample <= 8
			sample_format( bit_count bits_per_sample, channel_count channels ) :
				_bits_per_sample( bits_per_sample ),
				_endianness( byte_order::little_endian ),
				_channels( channels ),
				_bytes_per_frame( (_bits_per_sample / 8) * _channels )
			{
				if( _bits_per_sample > 8 ) {
					throw byte_order_exception{};
				}
			}

			/// Construct an sample format, with more than eight bits per sample.
			///
			/// @param bits_per_sample   The number of bits that each sample
			///                          consists of. This value must be >8
			/// @param endianness        The byte order of each sample
			/// @param channels          The number of interleaved channels
			/// @pre bits_per_sample > 8
			sample_format( bit_count bits_per_sample, byte_order endianness, channel_count channels ) :
				_bits_per_sample( bits_per_sample ),
				_endianness( endianness ),
				_channels( channels ),
				_bytes_per_frame( (_bits_per_sample / 8) * _channels )
			{
				if( _bits_per_sample <= 8 ) {
					throw byte_order_exception{};
				}
			}

			/// Retrieve the bits per sample of the sample format
			/// @returns The number of bits per individual sample
			bit_count bits_per_sample() const {
				return _bits_per_sample;
			}

			/// Retrieve the endianness of the sample format
			/// @returns The byte order of each individual sample
			/// @throws byte_order_exception is thrown if the format has samples
			///                              that is less or equal to one byte
			///                              size. Byte order does not apply
			///                              unless the samples are of multiple
			///                              bytes.
			/// @pre this.bits_per_sample() > 8
			byte_order endianness() const {
				if( _bits_per_sample <=8 ) {
					throw byte_order_exception{};
				}
				return _endianness;
			}

			/// Retrieve the number channels of the sample format
			/// @returns The number of interleaved channels
			channel_count channels() const {
				return _channels;
			}

			///
			bool operator==( sample_format const& lhs ) const {
				return _bits_per_sample == lhs._bits_per_sample &&
				       _endianness == lhs._endianness && 
				       _channels == lhs._channels;
			}

			///
			bool operator!=( sample_format const& lhs ) const {
				return !(*this == lhs);
			}

			///
			byte_count bytes_per_frame() const {
				return _bytes_per_frame;
			}

		private:
			bit_count _bits_per_sample;    ///< The number of bits per individual sample
			byte_order _endianness;        ///< The byte order of each sample
			channel_count _channels;       ///< The number of interleaved channels
			byte_count _bytes_per_frame;   ///< The number of bytes per frame
	};


	// Constants for common sample formats
	extern sample_format const eight_bits_mono;
	extern sample_format const eight_bits_stereo;
	extern sample_format const sixteen_bits_little_endian_mono;
	extern sample_format const sixteen_bits_big_endian_mono;
	extern sample_format const sixteen_bits_little_endian_stereo;
	extern sample_format const sixteen_bits_big_endian_stereo;
}

#endif   // IG_CHIRP_SAMPLE_FORMAT_HPP