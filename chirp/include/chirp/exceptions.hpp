#ifndef IG_CHIRP_EXCEPTIONS_HPP
#define IG_CHIRP_EXCEPTIONS_HPP

#include <stdexcept>

namespace chirp
{
	// Exception type for error conditions in the chirp library
	struct exception : std::exception {};

	/// Exception type for error conditions related to byte ordering
	struct byte_order_exception : exception {};

}   // namespace chirp

#endif   // IG_CHIRP_EXCEPTIONS_HPP