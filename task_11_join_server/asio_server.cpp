#include "asio_server.h"

void bulk_server::client_session(bulk_client_ptr client)
{
	char recv_data[512];
	string buffer = "";

	while (true)
	{
		size_t len = 0;

		error_code ec;
		len = client->m_sock->receive(asio::buffer(recv_data), 0, ec);

		if (len)
			buffer.append(recv_data, len);

		if (buffer.find('\n') != string::npos)
		{
			vector<string> cmd_vec = str_tool::split(buffer, "\n");

			for (string cmd : cmd_vec)
			{
				//Remove all escape characters from buffer
				str_tool::replace_all(cmd, "\r", "");
				str_tool::replace_all(cmd, "\n", "");

				if (cmd == "")
					continue;

				log << "<" << cmd << "\r\n";

				string answer = m_cmd_dispatcher->execute(cmd);
				client->m_sock->send(asio::buffer(answer));

				log << ">" << answer << "\r\n";
			}

			buffer.clear();
		}

		if (ec.value())
		{
			//Client fell off
			{
				lock_guard<mutex> lock(m_list_lock);
				m_client_list.remove(client);
			}

			//If connection aborted not from server end - detach thread
			if (ec.value() != asio::error::connection_aborted)
				client->m_serve_th.detach();

			log << "Client fell off\r\n";
			return;
		}
	}
}

void bulk_server::accept_handler(	const error_code& error,
									socket_ptr sock,
									asio::ip::tcp::acceptor& acceptor)
{
	if (error)
		return;

	log << "New client connected\r\n";

	bulk_client_ptr client(new bulk_client());

	{
		lock_guard<mutex> lock(m_list_lock);
		client->m_sock = sock;
		client->m_serve_th = thread(&bulk_server::client_session, this, client);
		m_client_list.push_back(client);
	}

	start_accept(acceptor);
}

void bulk_server::start_accept(asio::ip::tcp::acceptor& acc)
{
	socket_ptr sock(new socket(m_io_context));

	acc.async_accept(	*sock, std::bind(&bulk_server::accept_handler, this,
						std::placeholders::_1,
						sock,
						std::ref(acc)));
}

void bulk_server::server_thread()
{
	asio::ip::tcp::endpoint ep(asio::ip::tcp::v4(), m_port);
	asio::ip::tcp::acceptor acc(m_io_context, ep);

	start_accept(acc);
	m_io_context.run();
}

bulk_server::bulk_server(uint16_t port, shared_ptr<ICmd_dispatcher> cmd_dispatcher)
{
	m_port = port;
	m_cmd_dispatcher = cmd_dispatcher;
}

bulk_server::~bulk_server()
{
	if (m_started)
		stop();
}

void bulk_server::set_verbose_out(bool enable)
{
	if (enable)
		log.enable();
	else
		log.disable();
}

bool bulk_server::start()
{
	m_started = true;

	log << "Start server thread\r\n";
	m_server_th = thread(&bulk_server::server_thread, this);
	return true;
}

void bulk_server::stop()
{
	log << "Stop server thread\r\n";

	m_io_context.stop();
	m_server_th.join();

	log << "Close client connections\r\n";

	while (true)
	{
		bulk_client_ptr client;

		{
			lock_guard<mutex> lock(m_list_lock);
			if (m_client_list.empty())
				break;

			//client remove itself from m_client_list in bulk_server::client_session
			//when m_sock connection closed
			client = m_client_list.front();
		}

		log << "Shutdown client\r\n";

		error_code ec = asio::error::connection_aborted;
		client->m_sock->close(ec);

		if (client->m_serve_th.joinable())
			client->m_serve_th.join();

		client->m_sock->shutdown(socket::shutdown_both, ec);
	}

	m_started = false;
}