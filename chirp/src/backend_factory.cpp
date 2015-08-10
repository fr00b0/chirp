#include <chirp/backend_factory.hpp>
#include <chirp/static_config.hpp>

// backend implementations
#include "directsound/directsound_backend.hpp"

namespace chirp
{
	namespace backend
	{
		// default constructor
		factory::factory() {
		}

		// create_backend()
		std::unique_ptr<platform> factory::create_platform( backend_identity request ) const {
			if( request == backend_identity::platform_default )	{
				request = default_backend();
			}
			switch( request ) {
				case backend_identity::directsound:
					return std::make_unique<backend::directsound_platform>();

				default:
					throw unknown_backend_exception{};
			};
		}



		// default_backend()
		backend_identity default_backend() {
#if defined(CHIRP_WITH_DIRECTSOUND)
			return backend_identity::directsound;
#else
			throw no_default_backend{};
#endif
		}
	}   // namespace backend
}   // namespace chirp