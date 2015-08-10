#include <catch.hpp>
#include <test_backend.hpp>
#include <chirp/output_device.hpp>
#include <type_traits>

SCENARIO( "output_device traits tests" ) {
	SECTION( "output_device is default constructible" ) {
		REQUIRE( (std::is_default_constructible<chirp::output_device>::value) == true );
	}
}

SCENARIO( "output_device can be created from a backend::output_device" ) {
	chirp::output_device device{ std::make_shared<test::output_device>() };
}

SCENARIO( "output_device gets it's name from the implementation" ) {
	GIVEN( "an ouptput device with a given immplementation and a name" ) {
		auto impl = std::make_shared<test::output_device>();
		impl->_name = "test name";
		chirp::output_device device{ impl };
		WHEN( "we get the name from the output_device" ) {
			auto name = device.name();
			THEN( "It's the same name as the implementation has" ) {
				REQUIRE( name == "test name" );
			}
		}
	}
}