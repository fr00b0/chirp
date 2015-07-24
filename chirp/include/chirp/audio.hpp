#ifndef IG_CHIRP_AUDIO_HPP
#define IG_CHIRP_AUDIO_HPP

namespace chirp
{

	/// 
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
