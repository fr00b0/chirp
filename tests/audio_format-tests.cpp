#include <catch.hpp>
#include <chirp/audio_format.hpp>
#include <type_traits>

SCENARIO( "audio_format traits checks" ) {
	SECTION( "Audio formats are not default constructible" ) {
		REQUIRE( (std::is_default_constructible<chirp::audio_format>::value) == false );
	}
	SECTION( "Audio formats are copy constructible" ) {
		REQUIRE( (std::is_copy_constructible<chirp::audio_format>::value) == true );
	}
	SECTION( "Audio formats are move constructible" ) {
		REQUIRE( (std::is_move_constructible<chirp::audio_format>::value) == true );
	}
	SECTION( "Audio formats are copy assignable" ) {
		REQUIRE( (std::is_copy_assignable<chirp::audio_format>::value) == true );
	}
	SECTION( "Audio formats are move assignable" ) {
		REQUIRE( (std::is_move_assignable<chirp::audio_format>::value) == true );
	}
}

SCENARIO( "audio_format can be created from a frequency and a sample_format" ) {
	GIVEN( "a audio_format created with the parameterized constructor") {
		chirp::audio_format format{ 44100, chirp::sixteen_bits_little_endian_stereo };
		WHEN( "we retrieve the frequency" ) {
			auto frequency = format.frequency();
			THEN( "it is equal to what we used in the constructor" ) {
				REQUIRE( frequency == 44100 );
			}
		}
		AND_WHEN( "we retrieve the sample format" ) {
			auto sformat = format.sample_format();
			THEN( "it is equal to what we used in the constructor" ) {
				REQUIRE( sformat == chirp::sixteen_bits_little_endian_stereo );
			}
		}
	}
}

SCENARIO( "audio_format has accessors for sample_format properties" ) {
	GIVEN( "a 16 bits audio format" ) {
		chirp::audio_format format{ 44100, {16, chirp::byte_order::little_endian, 2} };
		THEN( "we can access bits per sample" ) {
			REQUIRE( format.bits_per_sample() == 16 );
		}
		AND_THEN( "we can access byte order" ) {
			REQUIRE( format.endianness() == chirp::byte_order::little_endian );
		}
		AND_THEN( "we can access channel count" ) {
			REQUIRE( format.channels() == 2 );
		}
		AND_THEN( "we can access the sample size in bytes" ) {
			REQUIRE( format.sample_size_in_bytes() == 4 );
		}
	}
	GIVEN( "a 8 bits audio format" ) {
		chirp::audio_format format{ 44100, {8, 1} };
		THEN( "we can access bits per sample" ) {
			REQUIRE( format.bits_per_sample() == 8 );
		}
		AND_THEN( "we can't access byte order" ) {
			REQUIRE_THROWS_AS( format.endianness(), chirp::byte_order_exception );
		}
		AND_THEN( "we can access channel count" ) {
			REQUIRE( format.channels() == 1 );
		}
		AND_THEN( "we can access the sample size in bytes" ) {
			REQUIRE( format.sample_size_in_bytes() == 1 );
		}
	}
}

SCENARIO( "audio_formats can report bytes per second" ) {
	GIVEN( "a audio format" ) {
		chirp::audio_format format{ 44100, {16, chirp::byte_order::little_endian, 2} };
		THEN( "we can retrieve the bytes per second of the audio format" ) {
			REQUIRE( format.bytes_per_second() == 176400 );
		}
	}
}

SCENARIO( "audio formats can be compared with operator ==" ) {
	GIVEN( "two equivalent sample formats" ) {
		chirp::audio_format format1{ 44100, chirp::sixteen_bits_little_endian_stereo };
		chirp::audio_format format2{ 44100, chirp::sixteen_bits_little_endian_stereo };
		THEN( "they are considered equivalent" ) {
			REQUIRE( (format1 == format2) == true );
		}
	}
	GIVEN( "two non-equivalent sample formats" ) {
		chirp::audio_format format1{ 44100, chirp::sixteen_bits_little_endian_stereo };
		chirp::audio_format format2{ 44100, chirp::eight_bits_mono };
		THEN( "they are not considered equivalent" ) {
			REQUIRE( (format1 == format2) == false );
		}
	}
}