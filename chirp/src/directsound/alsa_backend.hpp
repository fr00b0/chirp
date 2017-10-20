#ifndef IG_CHRIP_SRC_ALSA_BACKEND_HPP
#define IG_CHRIP_SRC_ALSA_BACKEND_HPP

#include <chirp/static_config.hpp>

#if defined(CHIRP_WITH_ALSA)

#include <chirp/backend.hpp>
#include <chirp/exceptions.hpp>
#include <chirp/sample_request.hpp>

#include <memory>

namespace chirp
{
	namespace backend
	{
		struct alsa_exception :
			chirp::backend_exception
		{};


		/// RAII wrwapper for a 
		class alsa_playback_handle
		{
			public:
				alsa_playback_handle()
			private:

		};


		class alsa_audio_stream :
			public backend::audio_stream
		{
			public:
				alsa_audio_stream();
				~alsa_audio_stream();

				///
				audio_stream_state state() const override;

				///
				void play_async( sample_provider_func ) override;

				///
				virtual void stop() override;
		};


		class alsa_output_device :
			public backend::output_device
		{
			public:
				///
				std::string name() const override;

				///
				std::unique_ptr<audio_stream> create_audio_stream( audio_format const& format ) override;

				///
				bool operator==(output_device const& other) const override;
		};


		class alsa_platform :
			public backend::platform
		{
			public:
				// typedefs
				using alsa_output_device_ptr = std::shared_ptr<alsa_output_device>;
				using alsa_output_device_collection = std::vector<alsa_output_device_ptr>;

				/// Default constructor
				alsa_platform();
				/// Destructor
				~alsa_platform();


				/// Create an instance of the default output device
				output_device_ptr default_output_device() const override;

				/// @returns collection of output devices
				output_device_collection get_output_devices() const override;				


		};
	}
}

#endif // defined(CHIRP_WITH_ALSA)

#endif // IG_CHIRP_SRC_ALSA_BACKEND_HPP
