#include "alsa_backend.hpp"

#if defined(CHIRP_WITH_ALSA)

#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

#include <iostream> // TODO: remove

#define UNIMPLEMENTED throw std::runtime_error{"unimplemented"};

namespace {

	/**
	 * RAII-wrapper for a string from the ALSA api
	 *
	 * The wrapper will free the string using a call to `::free` in the
	 * destructor.
	 */
	class alsa_string
	{
		public:
			/// Constrcut an alsa_string instance
			/// @param str   Pointer to string as retrieved from the ALSA api,
			///              where we are instructed to free the string with a 
			///              call to `::free`
			alsa_string(char* str) :
				_str(str)
			{}

			/// Destructor. Will release the string with a call to `::free`
			~alsa_string()
			{
				if( _str != nullptr ) {
					::free(_str);
				}
			}

			/// @returns The string as a `std::string`. If the contained string
			///          is missing (nullptr), then the returned string will
			///          be an empty string.
			std::string as_string() const {
				return _str!= nullptr ? std::string{_str} : std::string{};
			}

		private:
			/// The wrapped string pointer, as retrieved from the ALSA api
			char* _str;
	}; // class alsa_string

	/**
	 * Convert a string representation of an Input/Output identifier to IOID enum
	 *
	 * @param str   String representation. Validvalues are "Output", "Input" or "",
	 *              where the empty string is interpreted as IOID::Both
	 * @note  The string representation is case sensitive
	 */
	chirp::backend::IOID ioid_from_string(std::string const& str) {
		if( str.empty() ) {
			return chirp::backend::IOID::Both;
		}
		if( str == "Input" ) {
			return chirp::backend::IOID::Input;
		}
		if( str == "Output" ) {
			return chirp::backend::IOID::Output;
		}
		throw chirp::backend::alsa_exception{};
	} // ioid_from_string()

	/**
	 * RAII-wrapper for the array retrieved from calling the ALSA api funciton `::snd_device_name_hint()`
	 *
	 * The wrapper will release the array in the destructor.
	 */
	class device_name_hints
	{
		public:
			/// Construct a device_name_hints instance by calling `::snd_device_name_hints()`
			///
			/// @param card    The card parameter to the call to `::snd_device_name_hints()`
			/// @param iface   The iface parameter to the call to `::snd_device_name_hints()`
			device_name_hints(int card, char const* iface) :
				_hints(nullptr)
			{
				if( 0 != ::snd_device_name_hint(card, iface, &_hints) ) {
					throw chirp::backend::alsa_exception{};
				}
			}

			/// Destructor
			/// Releses the hints by calling `::snd_device_name_free_hint()`
			~device_name_hints()
			{
				::snd_device_name_free_hint(_hints);
			}

			/// Retreive information about all devices that we got info about when constructing the instace
			/// @returns  vector of alsa_device_info objects describing all devices that we got
			///           hints about.
			std::vector<chirp::backend::alsa_device_info> all_devices() const {
				if( _hints == nullptr ) {
					return {};
				}
				std::vector<chirp::backend::alsa_device_info> info;
				auto hint = reinterpret_cast<char** const>(_hints);
				while(*hint != nullptr) {
					alsa_string name{::snd_device_name_get_hint(*hint, "NAME")};
					alsa_string description{::snd_device_name_get_hint(*hint, "DESC")};
					alsa_string ioid{::snd_device_name_get_hint(*hint, "IOID")};

					if( name.as_string() != "null" ) {
						info.emplace_back(name.as_string(), description.as_string(), ioid_from_string(ioid.as_string()));
					}

					++hint;
				}
				return info;
			}
		private:
			/// Wrapped hints array
			void** _hints;
	}; // class device_name_hints

	/**
	 * Factory function that creates alsa_output_device pointers to all pcm output devices
	 * @returns  Collection of shared pointer to alsa_output_device instances.
	 */
	chirp::backend::alsa_platform::alsa_output_device_collection all_pcm_output_devices() {
		device_name_hints hints{-1, "pcm"};
		chirp::backend::alsa_platform::alsa_output_device_collection output_devices;
		for( auto&& info : hints.all_devices() ) {
			if( info.is_output_device() ) {
				output_devices.push_back( std::make_shared<chirp::backend::alsa_output_device>(info) );
			}			
		}
		return output_devices;
	} // all_pcm_output_devices()

} // anonymous namespace

namespace chirp {
	namespace backend {

		// alsa_platform constructor
		alsa_platform::alsa_platform() :
			_output_devices(all_pcm_output_devices())
		{
		}

		// alsa_platform destructor
		alsa_platform::~alsa_platform()
		{}

		// default_output_device()
		alsa_platform::output_device_ptr alsa_platform::default_output_device() const {
			auto it = std::find_if(std::begin(_output_devices), std::end(_output_devices),
				[](auto const& device_ptr){
					return device_ptr && device_ptr->name() == "default";
				});
			if( it != std::end(_output_devices) ) {
				return *it;
			}
			return nullptr;
		}

		// get_output_devices()
		alsa_platform::output_device_collection alsa_platform::get_output_devices() const {
			return output_device_collection{ std::begin(_output_devices), std::end(_output_devices) };
		}

	} // namespace backend
} // namespace chirp

#else // defined(CHIRP_WITH_ALSA)

namespace chirp {
	namespace backend {
		int alsa_dummy() {
			// to avoid linker errors about translation units without symbols
			return 0; 
		} 
	} // namespace backend
} // namespace chirp

#endif // defined(CHIRP_WITH_ALSA)
