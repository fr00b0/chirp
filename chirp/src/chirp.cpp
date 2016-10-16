#include <chirp/chirp.hpp>
#include <chirp/exceptions.hpp>
#include <chirp/backend_factory.hpp>

namespace chirp
{
	sample_format const eight_bits_mono = { 8, 1 };
	sample_format const eight_bits_stereo = { 8, 2 };
	sample_format const sixteen_bits_little_endian_mono = { 16, byte_order::little_endian, 1 };
	sample_format const sixteen_bits_big_endian_mono = { 16, byte_order::big_endian, 1 };
	sample_format const sixteen_bits_little_endian_stereo = { 16, byte_order::little_endian, 2 };
	sample_format const sixteen_bits_big_endian_stereo = { 16, byte_order::big_endian, 2 };


	// constructor
	audio_platform::audio_platform( backend_identity request ) {
		backend::factory factory;
		_platform_ptr = factory.create_platform( request );
		if( _platform_ptr == nullptr ) {
			throw unknown_backend_exception{};
		}
	}

	// constructor
	audio_platform::audio_platform( std::unique_ptr<backend::platform> platform ) :
		_platform_ptr( std::move(platform) )
	{
		if( _platform_ptr == nullptr ) {
			throw unknown_backend_exception{};
		}
	}

	// default_output_device()
	output_device audio_platform::default_output_device() const {
		return output_device{ _platform_ptr->default_output_device() };
	}

	// get_output_devices()
	output_devices audio_platform::get_output_devices() const {
		auto devs = _platform_ptr->get_output_devices();
		return output_devices( std::begin(devs), std::end(devs) );
	}

}   // namespace chirp
