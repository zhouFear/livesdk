#include "tcpserver.h"
#include <boost/bind.hpp>


network::tcpserver::tcpserver(uint16_t port, tcpserverlistener* listener) 
	:m_listener(listener)
	,m_io_ser()
	,m_acceptor(m_io_ser)
{
	ASIOTCP::endpoint ep(ASIOTCP::v6(), port);
	boost::system::error_code ec;
	m_acceptor.open(ep.protocol(), ec);
	m_acceptor.set_option(ASIOTCP::acceptor::reuse_address(true), ec);
	m_acceptor.bind(ep);
	m_acceptor.accept();
	accept();
}

network::tcpserver::~tcpserver()
{

}

void network::tcpserver::run()
{
	m_io_ser.run();
}

void network::tcpserver::stop()
{
	m_io_ser.stop();
}

void network::tcpserver::accept()
{
	ASIOTCP::endpoint sock;
	boost::weak_ptr<tcpserver> _twptr(shared_from_this());
	m_acceptor.async_accept([_twptr](const boost::system::error_code& ec, ASIOTCP::socket socket) {
		boost::shared_ptr<tcpserver> _tsptr = _twptr.lock();
		if (_tsptr)
		{
			_tsptr->accept();

		}
	});
}
