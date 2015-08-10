#ifndef IG_CHIRP_BACKEND_HPP
#define IG_CHIRP_BACKEND_HPP

#include <chirp/audio_format.hpp>

#include <memory>
#include <string>

namespace chirp
{
	namespace backend
	{
		/// Interface for audio implementations
		///
		///
		class audio
		{
			public:
				/// Pure virtual destructor
				virtual ~audio() = 0;

				struct sample_provider
				{
					virtual void on_need_samples() = 0;

				};

		};

		/// Interface for output devices
		///
		///
		class output_device
		{
			public:
				using frequency_type = std::uint32_t;

				/// Pure virtual destructor
				virtual ~output_device() = 0;

				///
				///
				///
				virtual std::string name() const = 0;

				///
				///
				///
				virtual std::unique_ptr<audio> create_audio( audio_format const& format ) = 0;
		};

		/// Interface for audio platform implementations
		///
		///
		class platform
		{
			public:
				/// Pure virtual destructor
				virtual ~platform() = 0;

				///
				///
				///
				virtual std::shared_ptr<output_device> default_output_device() const = 0;


		};

		// destructor implementation
		inline platform::~platform() {
		}

		// destructor implementation
		inline audio::~audio() {
		}

		// destructor implementation
		inline output_device::~output_device() {
		}
	}   // namespace backend
}   // namespace chirp

#endif   // IG_CHIRP_BACKEND_HPP