#ifndef IG_CHIRP_AUDIO_HPP
#define IG_CHIRP_AUDIO_HPP

#include <chirp/backend.hpp>

namespace chirp
{

	/// Audio class.
	///
	/// Audio is something that can be played through an output device.
	/// Each audio instance is tied to the only instance of an output device
	/// that is able to play it, which is the same instance that was used
	/// to create the audio instance.
	class audio
	{
		public:

			/// Create an audio instance from a backend implementation
			/// @param ptr   Pointer to audio implementation
			audio( audio_format const& format, std::shared_ptr<backend::audio> const& ptr ) :
				_format( format ),
				_ptr( ptr )
			{}

			///
			///
			template <class F>
			void play_async( F func ) {
				struct provider :
					backend::audio::sample_provider
					{
						void on_need_samples() override
						{
						}
					};
				func;
			}

			/// Stop audio playback
			void stop() {
				throw "";
			}

			/// @returns the audio format
			audio_format const& format() const {
				return _format;
			}
		private:
			/// The audio format
			audio_format _format;
			/// Pointer to implementation
			std::shared_ptr<backend::audio> _ptr;
	};

}   // namespace chirp

#endif   // IG_CHIRP_AUDIO_HPP
