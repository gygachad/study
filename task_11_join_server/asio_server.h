#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <list>

#include <asio.hpp>

using namespace std;

#include "str_tool.h"

class logger
{
	bool m_enabled = false;

public:
	logger() {}

	void enable() { m_enabled = true; }
	void disable() { m_enabled = false; }

	logger& operator<<(const std::string& data)
	{
		if (m_enabled)
			cout << data;

		return *this;
	}
};

class ICmd_dispatcher
{
public:
	virtual std::string execute(std::string& cmd_line) = 0;
};

struct bulk_client
{
	using socket = asio::ip::tcp::socket;
	typedef shared_ptr<socket> socket_ptr;

	socket_ptr m_sock;
	thread m_serve_th;
};

class bulk_server
{
	using socket = asio::ip::tcp::socket;
	typedef shared_ptr<socket> socket_ptr;
	typedef shared_ptr<bulk_client> bulk_client_ptr;

	list<bulk_client_ptr> m_client_list;
	mutex m_list_lock;

	uint16_t m_port;
	size_t m_bulk_size;

	asio::io_context m_io_context;
	thread m_server_th;

	shared_ptr<ICmd_dispatcher> m_cmd_dispatcher;

	logger log;

	bool m_started = false;

	void client_session(bulk_client_ptr client);
	
	void accept_handler(const error_code& error,
						socket_ptr sock,
						asio::ip::tcp::acceptor& acceptor);

	void start_accept(asio::ip::tcp::acceptor& acc);
	
	void server_thread();

public:
	bulk_server(uint16_t port, shared_ptr<ICmd_dispatcher> cmd_dispatcher);
	~bulk_server();

	void set_verbose_out(bool enable);

	bool start();
	void stop();
};