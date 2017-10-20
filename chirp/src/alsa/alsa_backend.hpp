#ifndef IG_CHRIP_SRC_ALSA_BACKEND_HPP
#define IG_CHRIP_SRC_ALSA_BACKEND_HPP

#include <chirp/static_config.hpp>

#if defined(CHIRP_WITH_ALSA)

#include <chirp/backend.hpp>
#include <chirp/exceptions.hpp>
#include <chirp/sample_request.hpp>

#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>

#include <memory>

namespace chirp
{
	namespace backend
	{
		/**
		 * Exception type used by the alsa backend
		 */
		struct alsa_exception :
			chirp::backend_exception
		{};


		/// Input/Output Indentification
		enum class IOID {
			/// Device is Input device only			
			Input,
			/// Device is Ouput device only
			Output,
			/// Device is both Input and Ouput device
			Both
		};

		/**
		 * Information describing an ALSA device
		 */
		class alsa_device_info
		{
			public:
				/// Create an instance of alsa_device_info
				/// @param name          The name of the device
				/// @param description   The description of the device
				/// @param ioid          The Input/Output identification of the device
				alsa_device_info(std::string const& name, std::string const& description, IOID ioid) :
					_name(name),
					_description(description),
					_ioid(ioid)
				{}

				/// @returns the name of the device
				std::string const& name() const {
					return _name;
				}

				/// @returns the description of the device
				std::string const& description() const {
					return _description;
				}

				/// @returns the Input/Output identification of the device
				IOID ioid() const {
					return _ioid;
				}

				/// @returns `true` if the device has output capabilities, else false
				bool is_output_device() const {
					return _ioid == IOID::Output || _ioid == IOID::Both;
				}

				/// @returns `true` if the device has input capabilities, else false
				bool is_input_device() const {
					return _ioid == IOID::Input || _ioid == IOID::Both;
				}

			private:
				/// Device name
				std::string _name;
				/// Device descriptions
				std::string _description;
				/// Device Input/Output identification
				IOID _ioid;
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
				/// Construct a alsa_output_device instance
				/// @param info   Info about the alsa device
				alsa_output_device( alsa_device_info const& info ) :
					_info(info)
				{
				}

				/// @returns The name of the alsa output device
				std::string name() const override {
					return _info.name();
				}

				/// Create an audio stream with a given format
				/// @param format   The format of the audio stream to create
				std::unique_ptr<audio_stream> create_audio_stream( audio_format const& /*format*/ ) override {
					return nullptr;
				}

				/// Compare this device with another device
				bool operator==(output_device const& other) const override {					
					return name() == other.name();
				}

			private:
				/// Info about the alsa device
				alsa_device_info _info;
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

			private:
				alsa_output_device_collection _output_devices;


		};
	} // namespace backend
} // namespace chirp

#else   // defined(CHIRP_WITH_ALSA)

#include <chirp/backend.hpp> 
#include <chirp/exceptions.hpp>

namespace chirp {
	namespace backend {
		// The alsa_platform is undefined wihtout alsa
		class alsa_platform :
			public undefined_platform
		{};
	} // namespace backend
} // namespace chirp

#endif   // defined(CHIRP_WITH_ALSA)
#endif   // IG_CHIRP_SRC_ALSA_BACKEND_HPP
