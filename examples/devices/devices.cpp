#include <iostream>

#include <chirp/chirp.hpp>

int main(int, char**) {

	/// Let's create the autio platform, device and audio instance to use
	chirp::audio_platform platform;

	for( auto const& device : platform.get_output_devices() ) {
		std::cout << device.name();
		if( device == platform.default_output_device()) {
			std::cout << " [default]";
		}
		std::cout << std::endl;
	}

	return 0;
}
