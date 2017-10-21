#include "directsound_backend.hpp"

#if defined(CHIRP_WITH_DIRECTSOUND)

#define NOMINMAX
#include <Windows.h>
#include <algorithm>
#include <mutex>

namespace
{
	// Enumeration function for direct sound devices
	BOOL CALLBACK enumProc( LPGUID guid_ptr, LPCSTR name, LPCSTR /*module*/, LPVOID context_ptr ) {
		auto* devices_ptr = reinterpret_cast<chirp::backend::directsound_platform::output_device_collection*>(context_ptr);
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
	auto const BufferSize_seconds = std::chrono::seconds{2};
	auto const UpdateInterval = std::chrono::milliseconds{10};
	auto const WriteAheadLimit = std::chrono::milliseconds{500};

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
			_output_devices( get_directsound_output_devices() )
		{
		}
		// directsound_platform destructor
		directsound_platform::~directsound_platform() {
		}

		// directsound_platform::default_ouput_device()
		directsound_platform::output_device_ptr directsound_platform::default_output_device() const {
			auto it = std::find_if( std::begin(_output_devices), std::end(_output_devices),
				[]( auto const& device_ptr ){
					return device_ptr->guid() == DSDEVID_DefaultPlayback;
				});
			if( it == std::end(_output_devices) ) {
				throw directsound_exception{};
			}
			return *it;
		}

		// directsound_platform::get_output_devices()
		directsound_platform::output_device_collection directsound_platform::get_output_devices() const {
			return output_device_collection( std::begin(_output_devices), std::end(_output_devices) );
		}

		// directsound_platform::get_directsound_output_devices()
		directsound_platform::directsound_output_device_collection directsound_platform::get_directsound_output_devices() const {
			directsound_output_device_collection result;
			auto hr = ::DirectSoundEnumerateA(reinterpret_cast<LPDSENUMCALLBACKA>(enumProc), reinterpret_cast<LPVOID>(&result));
			if( FAILED(hr) ) {
				throw directsound_exception{};
			}
			return result;
		}

		//-----------------------------------------------------------------
		// directsound_output_device implementation
		//-----------------------------------------------------------------

		// directsound_output_device::create_audio_stream()
		std::unique_ptr<audio_stream> directsound_output_device::create_audio_stream( audio_format const& format ) {
			if( !_dsi ) {
				_dsi = directsound_instance{ _guid };
				if( FAILED(_dsi.ptr()->SetCooperativeLevel( ::GetDesktopWindow(), DSSCL_NORMAL)) ) {
					throw directsound_exception{};
				}
			}
			return std::make_unique<directsound_audio_stream>( *this, format );
		}

		// operator==()
		bool directsound_output_device::operator==(chirp::backend::output_device const& other ) const {
			auto ptr = dynamic_cast<directsound_output_device const*>(&other);
			return ptr != nullptr && ptr->_guid == _guid;
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
		// directsound_audio_stream implementation
		//-----------------------------------------------------------------

		// directsound_audio_stream::create_buffer()
		void directsound_audio_stream::create_buffer(directsound_instance& instance, audio_format const& format) {
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
			bufferDesc.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_GETCURRENTPOSITION2;
			bufferDesc.dwReserved = 0;
			bufferDesc.dwBufferBytes = static_cast<DWORD>( BufferSize_seconds.count() * format.bytes_per_second() );
			bufferDesc.guid3DAlgorithm = DS3DALG_DEFAULT;
			bufferDesc.lpwfxFormat = &waveFormat;
			LPDIRECTSOUNDBUFFER ptr = nullptr;
			if( FAILED(instance.ptr()->CreateSoundBuffer(&bufferDesc, &ptr, nullptr)) ) {
				throw directsound_exception{};
			}
			ptr->SetFormat(&waveFormat);
			_buffer = buffer_ptr{ ptr };

			clear_entire_buffer();
			_state = audio_stream_state::ready;
		}

		// directsound_audio_stream::clear_entire_buffer()
		void directsound_audio_stream::clear_entire_buffer() {
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

		// restore_lost_buffer()
		void directsound_audio_stream::restore_lost_buffer()
		{
			DWORD status = 0;
			if( !FAILED(_buffer->GetStatus(&status)) )
			{
				if( status & DSBSTATUS_BUFFERLOST )
				{
					if( !FAILED(_buffer->Restore()) ) {
						clear_entire_buffer();
					}
				}
			}
		}

		// update()
		void directsound_audio_stream::update( duration_type const& delta ) {
			delta;

			auto limit_duration_ms = static_cast<DWORD>(std::chrono::duration_cast<std::chrono::milliseconds>(WriteAheadLimit).count());
			auto limit_byte_count = static_cast<DWORD>((_format.bytes_per_second() * limit_duration_ms) / std::milli{}.den);
			auto buffer_bytes = static_cast<DWORD>(BufferSize_seconds.count()*_format.bytes_per_second());

			restore_lost_buffer();

			DWORD read_cursor = 0;
			DWORD write_cursor = 0;
			if( FAILED(_buffer->GetCurrentPosition(&read_cursor, &write_cursor) ) ) {
				stop();
			}
			else {
				// We want to write data to the buffer, from the last write position
				// (making sure we are beyond the write cursor of the buffer)
				// and to the play cursor (wrapping around), but we also want to
				// respect the write-ahead limitation.

				// Let's check if we need to wait until we can write more data to the buffer
				DWORD written_ahead_bytes = (_current_write_position >= read_cursor) ? (_current_write_position - read_cursor) : (buffer_bytes - read_cursor + _current_write_position);
				if( written_ahead_bytes >= limit_byte_count ) {
					return;
				}

				DWORD from = _current_write_position; //std::max<DWORD>( write_cursor, _current_write_position );
				DWORD unsafe_bytes = (read_cursor <= write_cursor) ? (write_cursor - read_cursor) : (buffer_bytes - read_cursor + write_cursor);
				DWORD byte_count =
					std::min<DWORD>(
						buffer_bytes - unsafe_bytes,
						limit_byte_count
					);

				void* ptr1 = nullptr;
				void* ptr2 = nullptr;
				DWORD size1 = 0;
				DWORD size2 = 0;
				if( !FAILED(_buffer->Lock( from, byte_count, &ptr1, &size1, &ptr2, &size2, 0 )) ) {
					issue_sample_request( ptr1, size1, buffer_bytes );
					if( ptr2 != nullptr ) {
						issue_sample_request( ptr2, size2, buffer_bytes );
					}
					_buffer->Unlock(ptr1, size1, ptr2, size2);
				}
			}
		}

		// issue_sample_request()
		void directsound_audio_stream::issue_sample_request( void* ptr, std::uint32_t size, std::uint32_t buffer_bytes ) {
			std::memset( ptr, 0, size );
			_sample_provider( _play_duration, sample_request{ptr, size, _format} );
			_play_duration += std::chrono::microseconds( (std::micro::den * size) / _format.bytes_per_second() );
			_current_write_position = _current_write_position + size;
			if( _current_write_position >= buffer_bytes ) {
				_current_write_position -= buffer_bytes;
			}
		}

		// play_async()
		void directsound_audio_stream::play_async( sample_provider_func f ) {
			std::unique_lock<std::mutex> lock{_mutex};
			_sample_provider = f;
			_device.ensure_play_thread_is_running();
			_connection = _device.on_update.connect( std::bind(&directsound_audio_stream::update, this, std::placeholders::_1) );
			auto hr = _buffer->Play(0, 0, DSBPLAY_LOOPING );
			if( FAILED(hr) ) {
				_buffer->Stop();
				_connection.disconnect();
				_state = audio_stream_state::invalid;
			}
			else {
				_state = audio_stream_state::playing;
			}
		}

		// stop()
		void directsound_audio_stream::stop() {
			std::unique_lock<std::mutex> lock{_mutex};
			if( _state == audio_stream_state::playing ) {
				_buffer->Stop();
				_connection.disconnect();
				_state = audio_stream_state::ready;
			}
		}

		// state()
		audio_stream_state directsound_audio_stream::state() const {
			if( _state == audio_stream_state::playing ) {
				DWORD status = 0;
				if( !FAILED( _buffer->GetStatus(&status) ) ) {
					if( status & DSBSTATUS_PLAYING ) {
						return audio_stream_state::playing;
					}
					return audio_stream_state::ready;
				}
				return audio_stream_state::invalid;
			}
			return _state;
		}
	}   // namespace backend
}   // namespace chirp
#else   // defined(CHIRP_WITH_DIRECTSOUND)

namespace {
	int dummy() {return 0;} // to avoid linker errors about translation units without symbols
}

#endif   // defined(CHIRP_WITH_DIRECTSOUND)
