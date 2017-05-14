#ifndef UV_PIPE_HPP
#define UV_PIPE_HPP

#include <functional>
#include <string>

#include <uv.h>

#include "Loop.hpp"
#include "Connect.hpp"
#include "Stream.hpp"

namespace uv
{
	class Pipe : public Stream<uv_pipe_t>
	{
	public:
		inline			Pipe(uv::Loop &loop, int ipc);

		inline int		open(uv_file file);
		inline int		bind(const std::string &name);
		inline int		connect(const std::string &name, std::function<void(uv::Pipe &)> handler);
		inline int		getsockname(char *buffer, size_t &size) const;
		inline int		getpeername(char *buffer, size_t &size) const;
		inline void		pendingInstances(int count);
		inline int		pendingCount();

		inline uv_handle_type	pendingType();

	private:
		std::function<void(uv::Pipe &)>	m_connectHandler = nullptr;
	};





	Pipe::Pipe(uv::Loop &loop, int ipc)
	{
		m_handle.data = this;
		uv_pipe_init(&loop.m_loop, &m_handle, ipc);
	}

	int Pipe::open(uv_file file)
	{
		return uv_pipe_open(&m_handle, file);
	}

	int Pipe::bind(const std::string &name)
	{
		return uv_pipe_bind(&m_handle, name.c_str());
	}
	
	int Pipe::connect(const std::string &name, std::function<void(uv::Pipe &)> handler)
	{
		auto connect = new (std::nothrow) Connect;
		if (connect == nullptr) return ENOMEM;

		m_connectHandler = handler;
		uv_pipe_connect(&connect->m_handle, &m_handle, name.c_str(),
			[](uv_connect_t *req, int status) {

			//connected
			if (!status) {
				auto &pipe_t = *reinterpret_cast<uv_pipe_t *>(req->handle);
				auto &pipe = *reinterpret_cast<uv::Pipe *>(pipe_t.data);
				pipe.m_connectHandler(pipe);
			}
			delete reinterpret_cast<Connect *>(req->data);
		});

		return 0;
	}

	int Pipe::getsockname(char *buffer, size_t &size) const
	{
		return uv_pipe_getsockname(&m_handle, buffer, &size);
	}

	int Pipe::getpeername(char *buffer, size_t &size) const
	{
		return uv_pipe_getpeername(&m_handle, buffer, &size);
	}

	void Pipe::pendingInstances(int count)
	{
		uv_pipe_pending_instances(&m_handle, count);
	}

	int Pipe::pendingCount()
	{
		return uv_pipe_pending_count(&m_handle);
	}

	uv_handle_type Pipe::pendingType()
	{
		return uv_pipe_pending_type(&m_handle);
	}
}


#endif
