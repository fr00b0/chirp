#ifndef IG_CHIRP_OUTPUT_DEVICES_HPP
#define IG_CHIRP_OUTPUT_DEVICES_HPP

#include <chirp/output_device.hpp>
#include <vector>

namespace chirp
{
	/// Type used for collections of `output_device_description` objects
	class output_devices
	{
			/// Type of container to use internally
			using device_container = std::vector<output_device>;
		public:
			/// Constant iterator type
			using const_iterator = device_container::const_iterator;
			/// Type of elements
			using value_type = device_container::value_type;
			/// Integral type to use for size
			using size_type = device_container::size_type;

			// Default constructable
			output_devices() = default;
			// Copyable
			output_devices& operator=( output_devices const& ) = default;

			/// Constructor with given content
			/// @tparam T        Type of content
			/// @param content   Content to initialize the container with
			/*
			template <class T>
			output_devices( T&& content ) :
				_devices( std::forward<T>(content) )
			{}
			*/

			/// Construct from iterator range
			template <class It>
			output_devices( It begin, It end ) :
				_devices( begin, end )
			{}

			/// Construct from initializer list containing output device
			/// descriptions.
			/// @param descriptions   Output device descriptions
			output_devices( std::initializer_list<value_type> const& descriptions  ) :
				_devices( descriptions )
			{
			}

			/// Returns an iterator to the beginning
			/// @returns Iterator to the beginning
			const_iterator begin() const {
				return _devices.begin();
			}

			/// Returns an iterator to the end
			/// @returns Iterator to the end
			const_iterator end() const {
				return _devices.end();
			}

			/// Returns the number of device descriptions
			/// @returns Number of device descriptions in the container
			size_type size() const {
				return _devices.size();
			}

			/// Checks wether the container is empty
			/// @returns `true`if the container is empty, `false` otherwise
			bool empty() const {
				return _devices.empty();
			}

		private:
			/// Underlying container of device descriptions
			device_container _devices;
	};	
}   // namespace chirp

#endif   // IG_CHIRP_OUTPUT_DEVICES_HPP