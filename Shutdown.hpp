#ifndef UV_SHUTDOWN_HPP
#define UV_SHUTDOWN_HPP

#include <uv.h>

#include "Req.hpp"

namespace uv
{
	template<typename T> class Stream;

	class Shutdown : public Req<uv_shutdown_t>
	{
	public:
		inline Shutdown();

	private:
		template<typename T> friend class Stream;
	};





	Shutdown::Shutdown()
	{
		m_handle.data = this;
	}
}


#endif
