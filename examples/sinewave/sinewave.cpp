#include <iostream>

#include <chirp/chirp.hpp>
#include <thread>
#include <iostream>
#include <limits>
#include <cmath>

/// Double PI (360 degrees in radians)
const float two_pi = 2.0f * 3.14159265358979f;
/// Default frequency, if none is specified on the command line
const int default_frequency = 440;
/// Default duration, if none is specified on the command line
const auto default_duration = std::chrono::milliseconds{3000};

/// Command line arguments
class arguments
{
	public:
		/// Parse command line
		arguments( int argc, char const* argv[] ) :
			m_frequency( default_frequency ),
			m_duration( default_duration )
		{
			if( argc > 1 ) {
				m_frequency = std::atoi( argv[1] );
			}
			if( argc > 2 ) {
				m_duration = std::chrono::milliseconds{ std::atoi( argv[2] ) };
			}
		}

		/// @returns Frequency
		int frequency() const {
			return m_frequency;
		}

		/// @returns Duration
		std::chrono::milliseconds duration() const {
			return m_duration;
		}

	private:
		/// Sine wave frequency (in hertz)
		int m_frequency;
		/// Duration to play the generated sine wave
		std::chrono::milliseconds m_duration;
};

/// Generate a sample of a sinewave
/// @param frequency   The frequency of the sinewave to generate (in hertz)
/// @param time        The samples location on the time axis of sinewave
std::int16_t sinewave_sample( int frequency, chirp::duration_type time )
{
	auto seconds = std::chrono::duration_cast< std::chrono::duration<float> >(time).count();
	auto value = std::sinf( frequency * two_pi * seconds );
	return static_cast<std::int16_t>( std::numeric_limits<std::int16_t>::max() * value );
}

///
/// Main entry point
///
int main( int argc, char const* argv[] ) {
	try {
		arguments args{ argc, argv };

		/// Let's create the autio platform, device and audio instance to use
		chirp::audio_platform platform;
		auto device = platform.default_output_device();
		auto audio = device.create_audio( {44100, chirp::sixteen_bits_little_endian_mono} );

		// Let's start playing (and generating) the audio 
		audio.play_async(
			[&args]( chirp::duration_type play_time, chirp::sample_request const& request ) {
				auto* ptr = static_cast<std::int16_t*>(request.buffer_start());
				for( std::uint32_t i=0; i<request.frames(); ++i ) {
					*(ptr++) = sinewave_sample( args.frequency(), play_time + (i*request.format().duration_per_frame() ) );
				}
			});

		std::this_thread::sleep_for( args.duration() );
	}
	catch( std::exception& e ) {
		std::cerr << "Exception:\n"
		          << "\tType:   " << typeid(e).name() << "\n"
		          << "\tMessage:" << e.what() << std::endl;
	}
	return 0;
}