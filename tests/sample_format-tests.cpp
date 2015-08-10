#include <catch.hpp>
#include <chirp/sample_format.hpp>
#include <type_traits>

SCENARIO( "sample_format traits checks" ) {
	SECTION( "sample formats are not default constructible" ) {
		REQUIRE( (std::is_default_constructible<chirp::sample_format>::value) == false );
	}
	SECTION( "sample formats are copy constructible" ) {
		REQUIRE( (std::is_copy_constructible<chirp::sample_format>::value) == true );
	}
	SECTION( "sample formats are move constructible" ) {
		REQUIRE( (std::is_move_constructible<chirp::sample_format>::value) == true );
	}
	SECTION( "sample formats are copy assignable" ) {
		REQUIRE( (std::is_copy_assignable<chirp::sample_format>::value) == true );
	}
	SECTION( "sample formats are move assignable" ) {
		REQUIRE( (std::is_move_assignable<chirp::sample_format>::value) == true );
	}
}

SCENARIO( "sample formats can be compared with operator ==" ) {
	GIVEN( "two equivalent sample formats" ) {
		chirp::sample_format format1{ 24, chirp::byte_order::big_endian, 1 };
		chirp::sample_format format2{ 24, chirp::byte_order::big_endian, 1 };
		THEN( "they are considered equivalent" ) {
			REQUIRE( (format1 == format2) == true );
		}
	}
	GIVEN( "two non-equivalent sample formats" ) {
		chirp::sample_format format1{ 16, chirp::byte_order::big_endian, 1 };
		chirp::sample_format format2{ 8, 2 };
		THEN( "they are not considered equivalent" ) {
			REQUIRE( (format1 == format2) == false );
		}
	}
}

SCENARIO( "sample formats can be compared with operator !=" ) {
	GIVEN( "two equivalent sample formats" ) {
		chirp::sample_format format1{ 24, chirp::byte_order::big_endian, 1 };
		chirp::sample_format format2{ 24, chirp::byte_order::big_endian, 1 };
		THEN( "they are not considered equivalent" ) {
			REQUIRE( (format1 != format2) == false );
		}
	}
	GIVEN( "two non-equivalent sample formats" ) {
		chirp::sample_format format1{ 16, chirp::byte_order::big_endian, 1 };
		chirp::sample_format format2{ 8, 2 };
		THEN( "they are considered equivalent" ) {
			REQUIRE( (format1 != format2) == true );
		}
	}
}

SCENARIO( "sample formats implements a copy constructor" ) {
	GIVEN( "an sample format" ) {
		chirp::sample_format format{ 16, chirp::byte_order::little_endian, 2 };
		WHEN( "we copy-construct a new sample format from the sample format" ) {
			chirp::sample_format copy{ format };
			THEN( "They are the equivalent" ) {
				REQUIRE( format == copy );
			}
		}
	}
}

SCENARIO( "sample formats implements a move constructor" ) {
	GIVEN( "an sample format" ) {
		chirp::sample_format format{ 16, chirp::byte_order::little_endian, 2 };
		WHEN( "we move-construct a new sample format from the sample format" ) {
			chirp::sample_format moved{ std::move(format) };
			THEN( "The new format has the properties of the old one" ) {
				REQUIRE( moved.bits_per_sample() == 16 );
				REQUIRE( moved.endianness() == chirp::byte_order::little_endian );
				REQUIRE( moved.channels() == 2 );
			}
		}
	}
}

SCENARIO( "sample formats implements a copy assign operator" ) {
	GIVEN( "an sample format" ) {
		chirp::sample_format format{ 16, chirp::byte_order::little_endian, 2 };
		WHEN( "we copy-assign a new sample format from the sample format" ) {
			chirp::sample_format copy{8,1};	//ignore parameters, sample_format is not default constructible
			copy = format;
			THEN( "They are the equivalent" ) {
				REQUIRE( format == copy );
			}
		}
	}
}

SCENARIO( "sample formats implements a move assign operator" ) {
	GIVEN( "an sample format" ) {
		chirp::sample_format format{ 16, chirp::byte_order::little_endian, 2 };
		WHEN( "we move-assign a new sample format from the sample format" ) {
			chirp::sample_format moved{8,1};	//ignore parameters, sample_format is not default constructible;
			moved = std::move(format);
			THEN( "The new format has the properties of the old one" ) {
				REQUIRE( moved.bits_per_sample() == 16 );
				REQUIRE( moved.endianness() == chirp::byte_order::little_endian );
				REQUIRE( moved.channels() == 2 );
			}
		}
	}
}

SCENARIO( "sample formats have parameterized constructor" ) {

	SECTION( "Constructor with byte order can't be used with bit_count <= 8" ) {
		REQUIRE_THROWS_AS( (chirp::sample_format{ 8, chirp::byte_order::little_endian, 1 }), chirp::byte_order_exception );
		REQUIRE_THROWS_AS( (chirp::sample_format{ 8, chirp::byte_order::big_endian, 1 }), chirp::byte_order_exception );
	}
	SECTION( "Constructor without byte order can't be used with bit_count >8" ) {
		REQUIRE_THROWS_AS( (chirp::sample_format{ 16, 1 }), chirp::byte_order_exception );
	}
	SECTION( "Parameters (bits_per_sample, endianness, channels) can be used with bit_count > 8" ) {
		chirp::sample_format format{ 16, chirp::byte_order::little_endian, 2 };
		REQUIRE( format.bits_per_sample() == 16 );
		REQUIRE( format.endianness() == chirp::byte_order::little_endian );
		REQUIRE( format.channels() == 2 );
	}
}

SCENARIO( "Byte order is not valid for bit_count <=8 formats" ) {
	GIVEN( "a 8 bit sample format" ) {
		chirp::sample_format format{ 8, 1 };
		THEN( "The method that retrieves the byte order of samples will throw an byte_order_exception" ) {
			REQUIRE_THROWS_AS( format.endianness(), chirp::byte_order_exception );
		}
	}
}

SCENARIO( "sample_format can report it's sample size, in bytes." ) {
	GIVEN( "a 8 bit mono sample format" ) {
		chirp::sample_format format{ 8, 1 };
		THEN( "the sample size is 1" ) {
			REQUIRE( format.sample_size_in_bytes() == 1 );
		}
	}
	GIVEN( "a 8 bit stereo sample format" ) {
		chirp::sample_format format{ 8, 2 };
		THEN( "the sample size is 2" ) {
			REQUIRE( format.sample_size_in_bytes() == 2 );
		}
	}
	GIVEN( "a 16 bit mono sample format" ) {
		chirp::sample_format format{ 16, chirp::byte_order::little_endian, 1 };
		THEN( "the sample size is 2" ) {
			REQUIRE( format.sample_size_in_bytes() == 2 );
		}
	}
	GIVEN( "a 16 bit stereo sample format" ) {
		chirp::sample_format format{ 16, chirp::byte_order::big_endian, 2 };
		THEN( "the sample size is 4" ) {
			REQUIRE( format.sample_size_in_bytes() == 4 );
		}
	}
}

SCENARIO( "Common formats is provided as constants" ) {
	SECTION( "8 bits, mono" ) {
		REQUIRE( chirp::eight_bits_mono.bits_per_sample() == 8 );
		REQUIRE_THROWS_AS( chirp::eight_bits_mono.endianness(), chirp::byte_order_exception );
		REQUIRE( chirp::eight_bits_mono.channels() == 1 );
	}
	SECTION( "8 bits, stereo" ) {
		REQUIRE( chirp::eight_bits_stereo.bits_per_sample() == 8 );
		REQUIRE_THROWS_AS( chirp::eight_bits_mono.endianness(), chirp::byte_order_exception );
		REQUIRE( chirp::eight_bits_stereo.channels() == 2 );
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