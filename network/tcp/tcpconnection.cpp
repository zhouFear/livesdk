#include "tcpconnection.h"

network::tcpconnection::tcpconnection(ASIOTCP::socket sock_)
	:m_sock(std::move(sock_)), m_data()
{

}

network::tcpconnection::~tcpconnection()
{

}

void network::tcpconnection::do_read()
{
	auto self(shared_from_this());
	m_sock.async_read_some(ASIO::buffer(m_data, MaxLen), [this, self](boost::system::error_code ec, std::size_t length) {
		if (!ec)
		{

		}
		});
}

void network::tcpconnection::do_write()
{
	auto self(shared_from_this());
	ASIO::async_write( m_sock, ASIO::buffer(m_data, MaxLen), [this, self](boost::system::error_code ec, std::size_t length) {
		if (!ec)
		{

		}
		});
}

