#ifndef IG_CHIRP_BACKEND_FACTORY_HPP
#define IG_CHIRP_BACKEND_FACTORY_HPP

#include <chirp/chirp.hpp>
#include <chirp/backend.hpp>

#include <memory>

namespace chirp
{
	namespace backend
	{
		class factory
		{
			public:
				factory();

				std::unique_ptr<platform> create_platform( backend_identity request ) const;
		};


		///
		///
		///
		///
		backend_identity default_backend();
	}
}   // namespace chirp

#endif   // IG_CHIRP_BACKEND_FACTORY_HPP