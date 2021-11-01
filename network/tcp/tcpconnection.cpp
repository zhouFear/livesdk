#include "tcpconnection.h"

network::tcpconnection::tcpconnection(t_socket_ptr _p_sock)
	:m_p_sock(_p_sock), m_data()
{

}

network::tcpconnection::~tcpconnection()
{

}

void network::tcpconnection::do_read()
{
	auto self(shared_from_this());
	m_p_sock->async_read_some(ASIO::buffer(m_data, MaxLen), [this, self](boost::system::error_code ec, std::size_t length) {
		if (!ec)
		{
			OutputDebugStringA(m_data);
			on_recv(m_data, length);
		}
		});
}

void network::tcpconnection::do_write()
{
	auto self(shared_from_this());
	ASIO::async_write( *m_p_sock, ASIO::buffer(m_data, MaxLen), [this, self](boost::system::error_code ec, std::size_t length) {
		if (!ec)
		{

		}
		});
}

void network::tcpconnection::on_recv(char* buffer, size_t length)
{

}

