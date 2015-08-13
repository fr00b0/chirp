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
			return std::make_unique<directsound_audio>( _dsi, format );
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
			waveFormat.nBlockAlign = static_cast<WORD>(format.sample_size_in_bytes());
			waveFormat.wBitsPerSample = format.bits_per_sample();
			waveFormat.cbSize = 0;

			DSBUFFERDESC bufferDesc;
			bufferDesc.dwSize = sizeof(DSBUFFERDESC);
			bufferDesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2;
			bufferDesc.dwReserved = 0;
			bufferDesc.dwBufferBytes = BufferSize_SampleCount * format.sample_size_in_bytes();
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

		//
		audio_state directsound_audio::state() const {
			return _state;
		}
	}   // namespace backend
}   // namespace chirp

#endif   // defined(CHIRP_WITH_DIRECTSOUND)