#include "directsound_backend.hpp"

#if defined(CHIRP_WITH_DIRECTSOUND)

#include <Windows.h>
#include <algorithm>

namespace
{
	// Enumeration function for direct sound devices
	BOOL CALLBACK enumProc( LPGUID guid_ptr, LPCSTR name, LPCSTR /*module*/, LPVOID context_ptr ) {
		auto* devices_ptr = reinterpret_cast<chirp::backend::directsound_platform::output_device_vector*>(context_ptr);
		if( devices_ptr == nullptr ) {
			return FALSE;
		}
		auto guid = DSDEVID_DefaultPlayback;
		if (guid_ptr) {
			guid = *guid_ptr;
		}
		auto device_ptr = std::make_shared<chirp::backend::directsound_output_device>( guid, name );
		devices_ptr->push_back( device_ptr );
		return TRUE;
	}


	// TEMPORARY CONSTANTS, these will get moved to some form of parameters
	auto UpdateInterval = std::chrono::milliseconds{10};


}   // anonymous namespace

namespace chirp
{
	namespace backend
	{
		//-----------------------------------------------------------------
		// directsound_platform implementation
		//-----------------------------------------------------------------

		// directsound_platform default constructor
		directsound_platform::directsound_platform() :
			_output_devices( enumerate_output_devices() )
		{
		}
		// directsound_platform destructor
		directsound_platform::~directsound_platform() {
		}

		// directsound_platform::default_ouput_device()
		std::shared_ptr<output_device> directsound_platform::default_output_device() const {
			auto it = std::find_if( std::begin(_output_devices), std::end(_output_devices),
				[]( output_device_ptr const& device_ptr ){
					return device_ptr->guid() == DSDEVID_DefaultPlayback;
				});
			if( it == std::end(_output_devices) ) {
				throw directsound_exception{};
			}
			return *it;
		}

		// directsound_platform::enumerate_output_devices()
		directsound_platform::output_device_vector directsound_platform::enumerate_output_devices() const {
			output_device_vector result;
			auto hr = ::DirectSoundEnumerateA(reinterpret_cast<LPDSENUMCALLBACKA>(enumProc), reinterpret_cast<LPVOID>(&result));
			if( FAILED(hr) ) {
				throw directsound_exception{};
			}
			return result;
		}

		//-----------------------------------------------------------------
		// directsound_output_device implementation
		//-----------------------------------------------------------------

		// directsound_output_device::create_audio()
		std::unique_ptr<audio> directsound_output_device::create_audio( audio_format const& format ) {
			if( !_dsi ) {
				_dsi = directsound_instance{ _guid };
				if( FAILED(_dsi.ptr()->SetCooperativeLevel(::GetDesktopWindow(), DSSCL_NORMAL)) ) {
					throw directsound_exception{};
				}
			}
			return std::make_unique<directsound_audio>( *this, format );
		}

		// ensure_play_thread_is_running()
		void directsound_output_device::ensure_play_thread_is_running() {
			if( _play_thread.joinable() ) {
				return;
			}

			// start the play thread
			_abort_play_thread = false;
			_play_thread = std::thread{
				[this]() {
					auto last_update = std::chrono::high_resolution_clock::now();
					while( !_abort_play_thread ) {
						std::this_thread::sleep_for( UpdateInterval );
						auto now = std::chrono::high_resolution_clock::now();
						this->on_update( std::chrono::duration_cast<chirp::duration_type>(now - last_update) );
						last_update = now;
					}
				}
			};
		}

		//-----------------------------------------------------------------
		// directsound_audio implementation
		//-----------------------------------------------------------------

		// directsound_audio::create_buffer()
		void directsound_audio::create_buffer(directsound_instance& instance, audio_format const& format) {
			WAVEFORMATEX waveFormat;
			waveFormat.wFormatTag = WAVE_FORMAT_PCM;
			waveFormat.nChannels = format.channels();
			waveFormat.nSamplesPerSec = format.frequency();
			waveFormat.nAvgBytesPerSec = format.bytes_per_second();
			waveFormat.nBlockAlign = static_cast<WORD>(format.bytes_per_frame());
			waveFormat.wBitsPerSample = format.bits_per_sample();
			waveFormat.cbSize = 0;

			DSBUFFERDESC bufferDesc;
			bufferDesc.dwSize = sizeof(DSBUFFERDESC);
			bufferDesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2;
			bufferDesc.dwReserved = 0;
			bufferDesc.dwBufferBytes = BufferSize_SampleCount * format.bytes_per_frame();
			bufferDesc.guid3DAlgorithm = DS3DALG_DEFAULT;
			bufferDesc.lpwfxFormat = &waveFormat;
			LPDIRECTSOUNDBUFFER ptr = nullptr;
			if( FAILED(instance.ptr()->CreateSoundBuffer(&bufferDesc, &ptr, nullptr)) ) {
				throw directsound_exception{};
			}
			_buffer = buffer_ptr{ ptr };

			clear_entire_buffer();
			_state = audio_state::ready;
		}

		// directsound_audio::clear_entire_buffer()
		void directsound_audio::clear_entire_buffer() {
			LPVOID bytes1_ptr = nullptr;
			DWORD bytes1_count = 0;
			LPVOID bytes2_ptr = nullptr;
			DWORD bytes2_count = 0;

			if( FAILED(_buffer->Lock(0, 0, &bytes1_ptr, &bytes1_count, &bytes2_ptr, &bytes2_count, DSBLOCK_ENTIREBUFFER)) ) {
				throw directsound_exception{};
			}

			std::memset(bytes1_ptr, 0, bytes1_count);
			if( bytes2_ptr != nullptr ) {
				std::memset(bytes2_ptr, 0, bytes2_count);
			}

			if( FAILED(_buffer->Unlock(bytes1_ptr, bytes1_count, bytes2_ptr, bytes2_count)) ) {
				throw directsound_exception{};
			}
		}

		// update()
		void directsound_audio::update( duration_type const& delta ) {
			delta;
		}

		// play_async()
		void directsound_audio::play_async( sample_provider_func f ) {
			_device.ensure_play_thread_is_running();
			_connection = _device.on_update.connect( std::bind(&directsound_audio::update, this, std::placeholders::_1) );
			if( FAILED(_buffer->Play(0, 0, DSBPLAY_LOOPING )) ) {
				stop();
				_state = audio_state::invalid;
			}
			else {
				_state = audio_state::playing;
			}
		}

		// stop()
		void directsound_audio::stop() {
			_buffer->Stop();
			_connection.disconnect();
			if( _state != audio_state::invalid ) {
				_state = audio_state::ready;
			}
		}

		// state()
		audio_state directsound_audio::state() const {
			return _state;
		}
	}   // namespace backend
}   // namespace chirp

#endif   // defined(CHIRP_WITH_DIRECTSOUND)