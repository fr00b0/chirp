#include <catch.hpp>

#include <chirp/sample_request.hpp>

#include <memory>

SCENARIO( "sample_request traits checks" ) {
	SECTION( "sample requests are not default constructible" ) {
		REQUIRE( (std::is_default_constructible<chirp::sample_request>::value) == false );
	}
	SECTION( "sample requests are copy constructible" ) {
		REQUIRE( (std::is_copy_constructible<chirp::sample_request>::value) == true );
	}
	SECTION( "sample requests are move constructible" ) {
		REQUIRE( (std::is_move_constructible<chirp::sample_request>::value) == true );
	}
	SECTION( "sample requests are copy assignable" ) {
		REQUIRE( (std::is_copy_assignable<chirp::sample_request>::value) == true );
	}
	SECTION( "sample requests are move assignable" ) {
		REQUIRE( (std::is_move_assignable<chirp::sample_request>::value) == true );
	}
}

SCENARIO( "sample_request can be created" ) {
	GIVEN( "a buffer and an audio format" ) {
		chirp::audio_format format{ 44100, chirp::eight_bits_stereo };
		auto buffer = std::make_unique<std::uint8_t[]>(176400);
		WHEN( "we create a sample_request with said buffer and format" ) {
			chirp::sample_request request{ buffer.get(), 176400, format };
			THEN( "we can retrieve the start of the buffer through the sample_request" ) {
				REQUIRE( request.buffer_start() == buffer.get() );
			}
			THEN( "we can retrieve the end of the buffer through the sample_request" ) {
				REQUIRE( request.buffer_end() == (buffer.get()+176400) );
			}
			THEN( "we can retrieve the buffer size (in bytes) through the sample_request" ){
				REQUIRE( request.buffer_size() == 176400 );
			}
			THEN( "we can access the requested audio format") {
				REQUIRE( request.format() == format );
				AND_THEN( "it has not been copied" ) {
					REQUIRE( &request.format() == &format );
				}
			}
			THEN( "we can find out how many sample frames the request buffer can fit" ) {
				REQUIRE( request.frames() == 88200 );
			}
			THEN( "we can find out the duration of the request buffer" ) {
				REQUIRE( request.duration() == std::chrono::seconds(2) );
			}
		}
	}
}