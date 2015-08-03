#include <chirp/backend_factory.hpp>

namespace chirp
{
	namespace backend
	{
		// default constructor
		factory::factory() {			
		}

		// create_backend()
		std::unique_ptr<platform> factory::create_platform( backend_identity request ) const {
			request;
			throw "";
		}


		// default_backend()
		backend_identity default_backend() {
			throw "";
		}		
	}   // namespace backend
}   // namespace chirp