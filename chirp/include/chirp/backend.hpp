#ifndef IG_CHIRP_BACKEND_HPP
#define IG_CHIRP_BACKEND_HPP

namespace chirp
{
	namespace backend
	{
		/// Interface for audio platform implementations
		///
		///
		class platform
		{
			public:
				/// Pure virtual destructor
				virtual ~platform() = 0;


		};

		/// Interface for audio implementations
		///
		///
		class audio
		{
			public:
				/// Pure virtual destructor
				virtual ~audio() = 0;

		};

		/// Interface for output devices
		///
		///
		class output_device
		{
			public:
				/// Pure virtual destructor
				virtual ~output_device() = 0;
		};

	}   // namespace backend
}   // namespace chirp

#endif   // IG_CHIRP_BACKEND_HPP