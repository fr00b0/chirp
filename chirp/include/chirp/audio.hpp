#ifndef IG_CHIRP_AUDIO_HPP
#define IG_CHIRP_AUDIO_HPP

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
			///
			///
			template <class F>
			void play_async( F func ) {
				func;
				throw "";
			}

			void stop() {
				throw "";				
			}
	};

}   // namespace chirp

#endif   // IG_CHIRP_AUDIO_HPP
