#ifndef IG_CHIRP_EXCEPTIONS_HPP
#define IG_CHIRP_EXCEPTIONS_HPP

#include <stdexcept>

namespace chirp
{
	/// Exception type for error conditions in the chirp library
	struct exception : std::exception {};

	/// Exception type for errors while creating backend instances
	struct unknown_backend_exception : exception {};

	/// Exception type for the error condition when the current platform
	/// has no default backend implementation.
	struct no_default_backend : exception {};

	/// Exception type for error conditions related to byte ordering
	struct byte_order_exception : exception {};

	/// Exception type for errors emitted from the backend implementation
	struct backend_exception : exception {};

	/// Exception type for errors when creating output devices
	struct output_device_exception : exception {};

}   // namespace chirp

#endif   // IG_CHIRP_EXCEPTIONS_HPP
