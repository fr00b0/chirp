#ifndef IG_CHIRP_TESTS_TEST_BACKEND_HPP
#define IG_CHIRP_TESTS_TEST_BACKEND_HPP

#include <chirp/backend.hpp>

namespace test
{
	/// backend output_device used for testing
	struct output_device :
		public chirp::backend::output_device
	{
		std::string name() const override {
			return _name;
		}

		std::unique_ptr<chirp::backend::audio> create_audio( chirp::audio_format const& ) override {
			throw std::exception{};
		}

		std::string _name;
	};
}   // namespace test

#endif   // IG_CHIRP_TESTS_TEST_BACKEND_HPP