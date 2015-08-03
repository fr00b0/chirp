#include <catch.hpp>
#include <chirp/audio_format.hpp>
#include <type_traits>

SCENARIO( "audio_format traits checks" ) {
	SECTION( "Audio formats are not default constructible" ) {
		REQUIRE( (std::is_default_constructible<chirp::audio_format>::value) == false );
	}
	SECTION( "Aduio formats are copy constructible" ) {
		REQUIRE( (std::is_copy_constructible<chirp::audio_format>::value) == true );
	}
	SECTION( "Aduio formats are move constructible" ) {
		REQUIRE( (std::is_move_constructible<chirp::audio_format>::value) == true );
	}
	SECTION( "Aduio formats are copy assignable" ) {
		REQUIRE( (std::is_copy_assignable<chirp::audio_format>::value) == true );
	}
	SECTION( "Aduio formats are move assignable" ) {
		REQUIRE( (std::is_move_assignable<chirp::audio_format>::value) == true );
	}
}

SCENARIO( "Audio formats can be compared with operator ==" ) {
	GIVEN( "two equivalent audio formats" ) {
		chirp::audio_format format1{ 24, chirp::byte_order::big_endian, 1 };
		chirp::audio_format format2{ 24, chirp::byte_order::big_endian, 1 };
		THEN( "they are considered equivalent" ) {
			REQUIRE( (format1 == format2) == true );
		}
	}
	GIVEN( "two non-equivalent audio formats" ) {
		chirp::audio_format format1{ 16, chirp::byte_order::big_endian, 1 };
		chirp::audio_format format2{ 8, 2 };
		THEN( "they are not considered equivalent" ) {
			REQUIRE( (format1 == format2) == false );
		}
	}
}

SCENARIO( "Audio formats can be compared with operator !=" ) {
	GIVEN( "two equivalent audio formats" ) {
		chirp::audio_format format1{ 24, chirp::byte_order::big_endian, 1 };
		chirp::audio_format format2{ 24, chirp::byte_order::big_endian, 1 };
		THEN( "they are not considered equivalent" ) {
			REQUIRE( (format1 != format2) == false );
		}
	}
	GIVEN( "two non-equivalent audio formats" ) {
		chirp::audio_format format1{ 16, chirp::byte_order::big_endian, 1 };
		chirp::audio_format format2{ 8, 2 };
		THEN( "they are considered equivalent" ) {
			REQUIRE( (format1 != format2) == true );
		}
	}
}

SCENARIO( "Audio formats implements a copy constructor" ) {
	GIVEN( "an audio format" ) {
		chirp::audio_format format{ 16, chirp::byte_order::little_endian, 2 };
		WHEN( "we copy-construct a new audio format from the audio format" ) {
			chirp::audio_format copy{ format };
			THEN( "They are the equivalent" ) {
				REQUIRE( format == copy );
			}
		}
	}
}

SCENARIO( "Audio formats implements a move constructor" ) {
	GIVEN( "an audio format" ) {
		chirp::audio_format format{ 16, chirp::byte_order::little_endian, 2 };
		WHEN( "we move-construct a new audio format from the audio format" ) {
			chirp::audio_format moved{ std::move(format) };
			THEN( "The new format has the properties of the old one" ) {
				REQUIRE( moved.bits_per_sample() == 16 );
				REQUIRE( moved.endianness() == chirp::byte_order::little_endian );
				REQUIRE( moved.channels() == 2 );
			}
		}
	}
}

SCENARIO( "Audio formats implements a copy assign operator" ) {
	GIVEN( "an audio format" ) {
		chirp::audio_format format{ 16, chirp::byte_order::little_endian, 2 };
		WHEN( "we copy-assign a new audio format from the audio format" ) {
			chirp::audio_format copy{8,1};	//ignore parameters, audio_format is not default constructible
			copy = format;
			THEN( "They are the equivalent" ) {
				REQUIRE( format == copy );
			}
		}
	}
}

SCENARIO( "Audio formats implements a move assign operator" ) {
	GIVEN( "an audio format" ) {
		chirp::audio_format format{ 16, chirp::byte_order::little_endian, 2 };
		WHEN( "we move-assign a new audio format from the audio format" ) {
			chirp::audio_format moved{8,1};	//ignore parameters, audio_format is not default constructible;
			moved = std::move(format);
			THEN( "The new format has the properties of the old one" ) {
				REQUIRE( moved.bits_per_sample() == 16 );
				REQUIRE( moved.endianness() == chirp::byte_order::little_endian );
				REQUIRE( moved.channels() == 2 );
			}
		}
	}
}

SCENARIO( "Audio formats have parameterized constructor" ) {

	SECTION( "Constructor with byte order can't be used with bit_count <= 8" ) {
		REQUIRE_THROWS_AS( (chirp::audio_format{ 8, chirp::byte_order::little_endian, 1 }), chirp::byte_order_exception );
		REQUIRE_THROWS_AS( (chirp::audio_format{ 8, chirp::byte_order::big_endian, 1 }), chirp::byte_order_exception );
	}
	SECTION( "Constructor without byte order can't be used with bit_count >8" ) {
		REQUIRE_THROWS_AS( (chirp::audio_format{ 16, 1 }), chirp::byte_order_exception );
	}
	SECTION( "Parameters (bits_per_sample, endianness, channels) can be used with bit_count > 8" ) {
		chirp::audio_format format{ 16, chirp::byte_order::little_endian, 2 };
		REQUIRE( format.bits_per_sample() == 16 );
		REQUIRE( format.endianness() == chirp::byte_order::little_endian );
		REQUIRE( format.channels() == 2 );
	}
}

SCENARIO( "Byte order is not valid for bit_count <=8 formats" ) {
	GIVEN( "a 8 bit audio format" ) {
		chirp::audio_format format{ 8, 1 };
		THEN( "The method that retrieves the byte order of samples will throw an byte_order_exception" ) {
			REQUIRE_THROWS_AS( format.endianness(), chirp::byte_order_exception );
		}
	}
}

SCENARIO( "Common formats is provided as constants" ) {
	SECTION( "8 bit, mono" ) {
		REQUIRE( chirp::eight_bit_mono.bits_per_sample() == 8 );
		REQUIRE_THROWS_AS( chirp::eight_bit_mono.endianness(), chirp::byte_order_exception );
		REQUIRE( chirp::eight_bit_mono.channels() == 1 );
	}
	SECTION( "8 bit, stereo" ) {
		REQUIRE( chirp::eight_bit_stereo.bits_per_sample() == 8 );
		REQUIRE_THROWS_AS( chirp::eight_bit_mono.endianness(), chirp::byte_order_exception );
		REQUIRE( chirp::eight_bit_stereo.channels() == 2 );
	}
	SECTION( "16 bits, little endian, mono" ) {
		REQUIRE( chirp::sixteen_bits_little_endian_mono.bits_per_sample() == 16 );
		REQUIRE( chirp::sixteen_bits_little_endian_mono.endianness() == chirp::byte_order::little_endian );
		REQUIRE( chirp::sixteen_bits_little_endian_mono.channels() == 1 );
	}
	SECTION( "16 bits, big endian, mono" ) {
		REQUIRE( chirp::sixteen_bits_big_endian_mono.bits_per_sample() == 16 );
		REQUIRE( chirp::sixteen_bits_big_endian_mono.endianness() == chirp::byte_order::big_endian );
		REQUIRE( chirp::sixteen_bits_big_endian_mono.channels() == 1 );
	}
	SECTION( "16 bits, little endian, stereo" ) {
		REQUIRE( chirp::sixteen_bits_little_endian_stereo.bits_per_sample() == 16 );
		REQUIRE( chirp::sixteen_bits_little_endian_stereo.endianness() == chirp::byte_order::little_endian );
		REQUIRE( chirp::sixteen_bits_little_endian_stereo.channels() == 2 );
	}
	SECTION( "16 bits, big endian, stereo" ) {
		REQUIRE( chirp::sixteen_bits_big_endian_stereo.bits_per_sample() == 16 );
		REQUIRE( chirp::sixteen_bits_big_endian_stereo.endianness() == chirp::byte_order::big_endian );
		REQUIRE( chirp::sixteen_bits_big_endian_stereo.channels() == 2 );
	}
}