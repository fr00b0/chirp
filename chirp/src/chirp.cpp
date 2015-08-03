#include <chirp/chirp.hpp>

namespace chirp
{
	audio_format const eight_bit_mono = { 8, 1 };
	audio_format const eight_bit_stereo = { 8, 2 };
	audio_format const sixteen_bits_little_endian_mono = { 16, byte_order::little_endian, 1 };
	audio_format const sixteen_bits_big_endian_mono = { 16, byte_order::big_endian, 1 };
	audio_format const sixteen_bits_little_endian_stereo = { 16, byte_order::little_endian, 2 };
	audio_format const sixteen_bits_big_endian_stereo = { 16, byte_order::big_endian, 2 };
	

	// constructor
	audio_platform::audio_platform( backend_identity request ) {
		request;
	}


	// default_output_device()
	output_device audio_platform::default_output_device() const {
		throw "";		
	}

}   // namespace chirp
