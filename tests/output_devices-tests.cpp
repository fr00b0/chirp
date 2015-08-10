#include <catch.hpp>
#include <chirp/output_devices.hpp>
#include <algorithm>

SCENARIO( "collections of output devices can be default constructed" ) {
	 GIVEN( "a default constructed collection of output devices" ) {
		chirp::output_devices devices;
	 	THEN( "the collection has zero size and is considered empty" ) {
	 		REQUIRE( devices.size() == 0 );
	 		REQUIRE( devices.empty() == true );
	 	}
	 	AND_THEN( "the iteration range is empty" ) {
	 		REQUIRE( std::distance( std::begin(devices), std::end(devices) ) == 0 );
	 	}
	 }

}

SCENARIO( "collections of output devices can be constructed with information about devices" ) {
	GIVEN( "a collection of output devices constructed with information about devices" ) {
		chirp::output_devices devices{ {}, {} };
		THEN( "The size is non-zero and the container is not considered empty" ) {
			REQUIRE( devices.size() == 2 );
			REQUIRE( devices.empty() == false );
		}
	 	AND_THEN( "the iteration range has elements" ) {
	 		REQUIRE( std::distance( std::begin(devices), std::end(devices) ) == 2 );
	 	}
	}
}

SCENARIO( "collections of output devices can be assigned with device descriptons" ) {
	GIVEN( "a empty collection of output devices" ) {
		chirp::output_devices devices;
		WHEN( "we assign the output device collection with another non-empty collection" ) {
			devices = chirp::output_devices{ {}, {}, {} };

			THEN( "the size changes and it is no longer empty" ) {
				REQUIRE( devices.size() == 3 );
				REQUIRE( devices.empty() == false );
			}
		 	AND_THEN( "the iteration range has elements" ) {
		 		REQUIRE( std::distance( std::begin(devices), std::end(devices) ) == 3 );
		 	}
		}
	}
}