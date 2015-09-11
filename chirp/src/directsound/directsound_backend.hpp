#ifndef IG_CHRIP_SRC_DIRECTSOUND_BACKEND_HPP
#define IG_CHRIP_SRC_DIRECTSOUND_BACKEND_HPP

#include <chirp/static_config.hpp>

#if defined(CHIRP_WITH_DIRECTSOUND)

#include <chirp/backend.hpp>
#include <chirp/exceptions.hpp>
#include <chirp/sample_request.hpp>
#include <nod/nod.hpp>

#include <dsound.h>
#include <vector>
#include <string>
#include <cstdint>
#include <cstring>
#include <thread>
#include <atomic>

namespace chirp
{
	namespace backend
	{
		// Exceptions

		struct directsound_exception : chirp::backend_exception {};

		///
		///
		///
		///
		template <class T>
		struct release_deleter
		{
			void operator()( T* ptr ) const {
				if (ptr) {
					ptr->Release();
				}
			}
		};

		///
		///
		///
		class directsound_instance
		{
			public:
				directsound_instance( directsound_instance const& ) = delete;
				directsound_instance& operator=( directsound_instance const& ) = delete;

				/// Default constructor
				directsound_instance() :
					_ds8_ptr( nullptr )
				{}

				/// Parameterized constructor
				directsound_instance( GUID device_guid ) {
					if( FAILED(::DirectSoundCreate8( &device_guid, &_ds8_ptr, nullptr )) ) {
						throw directsound_exception{};
					}
				}

				/// Move constructor
				directsound_instance( directsound_instance&& other ) :
					_ds8_ptr( other._ds8_ptr )
				{
					other._ds8_ptr = nullptr;
				}

				/// destructor
				~directsound_instance() {
					release();
				}

				/// Move assingment
				directsound_instance& operator=( directsound_instance&& lhs )
				{
					if( this == &lhs ) {
						return *this;
					}
					release();
					std::swap( _ds8_ptr, lhs._ds8_ptr );
					return *this;
				}

				/// @returns pointer to the direct sound instance
				IDirectSound8* ptr() {
					return _ds8_ptr;
				}

				/// @returns const pointer to the direct sound instance
				IDirectSound8 const* ptr() const {
					return _ds8_ptr;
				}

				///
				bool operator!() const {
					return ptr() == nullptr;
				}

				/// Equality check
				bool operator==( directsound_instance const& lhs ) const {
					return ptr() == lhs.ptr();
				}

				/// Inequality check
				bool operator!=( directsound_instance const& lhs ) const {
					return !(*this == lhs);
				}

				///
				void release() {
					if( _ds8_ptr ) {
						_ds8_ptr->Release();
						_ds8_ptr = nullptr;
					}
				}

			private:

				/// Pointer to the active directsound instance
				IDirectSound8* _ds8_ptr;
		};

		///
		///
		///
		class directsound_output_device :
			public backend::output_device
		{
			public:
				/// Parameterized constructor
				/// @param guid   The guid of the device
				/// @param name   The name of the device
				directsound_output_device( GUID guid, std::string const& name ) :
					_guid( guid ),
					_name( name ),
					_abort_play_thread( false )
				{}

				/// Destructor
				~directsound_output_device() {
					_abort_play_thread = true;
					if( _play_thread.joinable() ) {
						_play_thread.join();
					}
				}

				/// @returns The device guid
				GUID guid() const {
					return _guid;
				}

				/// @returns The device name
				std::string name() const {
					return _name;
				}

				///
				///
				///
				std::unique_ptr<audio> create_audio( audio_format const& format ) override;

				///
				directsound_instance& directsound() {
					return _dsi;
				}

				///
				directsound_instance const& directsound() const {
					return _dsi;
				}

				///
				void ensure_play_thread_is_running();

				///
				nod::signal<void(duration_type const&)> on_update;

			private:
				/// The device guid
				GUID _guid;
				/// The device name
				std::string _name;
				/// The direct sound instance
				directsound_instance _dsi;
				/// Play thread
				std::thread _play_thread;
				/// Atomic flag for aborting the play thread
				std::atomic<bool> _abort_play_thread;
		};

		///
		///
		///
		class directsound_audio :
			public backend::audio
		{
			public:
				///
				directsound_audio(directsound_output_device& device, audio_format const& format) :
					_device(device),
					_format(format),
					_state(audio_state::invalid),
					_play_duration(0.0),
					_current_write_position(0)
				{
					create_buffer( _device.directsound(), format );
				}

				///
				~directsound_audio() {
					stop();
				}

				///
				audio_state state() const override;

				///
				void play_async( sample_provider_func f ) override;

				///
				void stop() override;

				///
				void update( duration_type const& delta );

			private:
				using buffer_ptr = std::unique_ptr<IDirectSoundBuffer, release_deleter<IDirectSoundBuffer>>;

				///
				///
				void create_buffer(directsound_instance& instance, audio_format const& format);

				/// Fill the entire buffer with zeros
				/// @throws directsound_exception is thrown if the buffer cannot be locked for writing.
				void clear_entire_buffer();

				///
				///
				void issue_sample_request( void* ptr, std::uint32_t size, std::uint32_t buffer_bytes );

				///
				void restore_lost_buffer();

				/// Device reference
				directsound_output_device& _device;
				/// Audio format
				audio_format _format;
				/// Pointer to the directsound buffer
				buffer_ptr _buffer;
				/// Current audio state
				std::atomic<audio_state> _state;
				/// Connection to the device update callback
				nod::scoped_connection _connection;
				/// The amount of time that has been played
				chirp::duration_type _play_duration;
				/// Mutex for syncronizing the internal state
				std::mutex _mutex;
				/// The buffer position where we stopped writing last time
				std::uint32_t _current_write_position;
				/// Current callback for handling sample requests
				sample_provider_func _sample_provider;
		};


		///
		///
		class directsound_platform :
			public backend::platform
		{
			public:
				// typedefs
				using output_device_ptr = std::shared_ptr<directsound_output_device>;
				using output_device_vector = std::vector<output_device_ptr>;

				/// Default constructor
				directsound_platform();
				/// Destructor
				~directsound_platform();


				///
				std::shared_ptr<output_device> default_output_device() const override;

			private:
				///
				///
				///
				output_device_vector enumerate_output_devices() const;

				/// Vector with all available output devices
				output_device_vector _output_devices;
		};
	}   // namespace backend
}   // namespace chirp

#endif   // defined(CHIRP_WITH_DIRECTSOUND)
#endif   // IG_CHRIP_DIRECTSOUND_BACKEND_HPP