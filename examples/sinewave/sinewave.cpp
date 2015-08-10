#include <iostream>

#include <chirp/chirp.hpp>
#include <thread>
#include <iostream>

///
/// Main entry point
///
int main() {
	try {
		chirp::audio_platform platform;
		auto device = platform.default_output_device();
		auto audio = device.create_audio( {44100, chirp::sixteen_bits_little_endian_stereo} );

		audio.play_async(
			[](  ) {
			});

		std::this_thread::sleep_for( std::chrono::milliseconds(1000) );
	}
	catch( std::exception& e ) {
		std::cerr << "Exception:\n"
		          << "\tType:   " << typeid(e).name() << "\n"
		          << "\tMessage:" << e.what() << std::endl;
	}
	return 0;
}