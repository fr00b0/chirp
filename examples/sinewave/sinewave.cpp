#include <iostream>

#include <chirp/chirp.hpp>
#include <thread>

///
/// Main entry point
///
int main() {
	chirp::audio_platform ap;
	auto device = ap.default_output_device();
	auto audio = device.create_audio( 44100, chirp::sixteen_bits_little_endian_stereo );

	audio.play_async(
		[](  ) {
			
		});

	std::this_thread::sleep_for( std::chrono::milliseconds(1000) );
	audio.stop();
	return 0;
}