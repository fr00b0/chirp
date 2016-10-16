#ifndef IG_CHIRP_BACKEND_HPP
#define IG_CHIRP_BACKEND_HPP

#include <chirp/audio_format.hpp>
#include <chirp/sample_request.hpp>

#include <memory>
#include <string>
#include <functional>
#include <vector>

namespace chirp
{
	namespace backend
	{
		/// Enumerator for keeping track of the state of audio_stream instances
		enum class audio_stream_state {
			/// The audio instance is in a invalid state, which means it is
			/// not ready to be played.
			invalid,
			/// The audio instance is ready to be played
			ready,
			/// The audio instance is playing
			playing
		};

		/// Interface for audio implementations
		///
		///
		class audio_stream
		{
			public:
				///
				using sample_provider_func = std::function<void(duration_type const&, sample_request const&)>;

				/// Pure virtual destructor
				virtual ~audio_stream() = 0;

				///
				virtual audio_stream_state state() const = 0;

				///
				virtual void play_async( sample_provider_func ) = 0;

				///
				virtual void stop() = 0;
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
				virtual std::unique_ptr<audio_stream> create_audio_stream( audio_format const& format ) = 0;

				///
				virtual bool operator==(output_device const& other) const = 0;
		};

		/// Interface for audio platform implementations
		///
		///
		class platform
		{
			public:
				using output_device_ptr = std::shared_ptr<output_device>;
				using output_device_collection = std::vector<output_device_ptr>;

				/// Pure virtual destructor
				virtual ~platform() = 0;

				///
				///
				///
				virtual output_device_ptr default_output_device() const = 0;

				virtual output_device_collection get_output_devices() const = 0;


		};

		// destructor implementation
		inline platform::~platform() {
		}

		// destructor implementation
		inline audio_stream::~audio_stream() {
		}

		// destructor implementation
		inline output_device::~output_device() {
		}
	}   // namespace backend
}   // namespace chirp

#endif   // IG_CHIRP_BACKEND_HPP