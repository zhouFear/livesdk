#include "tcpserver.h"
#include <boost/bind.hpp>
#include "tcpconnection.h"
#include "tcpconnectionmgr.h"


network::tcpserver::tcpserver(uint16_t port) 
	:m_io_ser()
	,m_acceptor(m_io_ser)
	, m_connection_manager_ptr(new tcpconnectionmgr())
{
	ASIOTCP::endpoint ep(ASIOTCP::v6(), port);
	boost::system::error_code ec;
	m_acceptor.open(ep.protocol(), ec);
	m_acceptor.set_option(ASIOTCP::acceptor::reuse_address(true), ec);
	m_acceptor.bind(ep);
	m_acceptor.listen();
}

network::tcpserver::~tcpserver()
{

}

void network::tcpserver::run()
{
	accept();
	m_io_ser.run();
}

void network::tcpserver::stop()
{
	m_io_ser.stop();
}

void network::tcpserver::accept()
{
	// ASIOTCP::endpoint sock;
	std::shared_ptr<tcpserver> self(shared_from_this());
	t_socket_ptr _ptr_sock = std::make_shared<ASIOTCP::socket>(m_io_ser);
	m_acceptor.async_accept(*_ptr_sock, [this, self, _ptr_sock](const boost::system::error_code& ec) {
		m_connection_manager_ptr->start(std::make_shared<network::tcpconnection>(_ptr_sock));
		accept();
	});
}
