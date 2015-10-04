#ifndef IG_CHIRP_AUDIO_STREAM_HPP
#define IG_CHIRP_AUDIO_STREAM_HPP

#include <chirp/backend.hpp>
#include <chirp/sample_request.hpp>

namespace chirp
{
	/// Audio stream class.
	///
	/// An audio stream is something that can be played through an output device.
	/// Each audio instance is tied to the only instance of an output device
	/// that is able to play it, which is the same instance that was used
	/// to create the audio instance.
	class audio_stream
	{
		public:

			/// Create an audio stream instance from a backend implementation
			/// @param ptr   Pointer to audio implementation
			audio_stream( audio_format const& format, std::shared_ptr<backend::audio_stream> const& ptr ) :
				_format( format ),
				_ptr( ptr )
			{}

			///
			///
			template <class F>
			void play_async( F func ) {
				_ptr->play_async( func );
			}

			/// Examin wether the audio stream is being played.
			///
			/// @returns `true` only if the audio stream is being played. All
			///          other states results in `false` being returned.
			bool is_playing() const {
				return _ptr && _ptr->state() == backend::audio_stream_state::playing;
			}

			/// Stop audio playback
			void stop() {
				_ptr->stop();
			}

			/// @returns the audio format
			audio_format const& format() const {
				return _format;
			}

		private:
			/// The audio format
			audio_format _format;
			/// Pointer to implementation
			std::shared_ptr<backend::audio_stream> _ptr;
	};
}   // namespace chirp

#endif   // IG_CHIRP_AUDIO_STREAM_HPP
