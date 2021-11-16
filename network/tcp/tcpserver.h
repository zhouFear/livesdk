/*! \file tcpserver.h */

#ifndef tcpserver_h__
#define tcpserver_h__
#include "../networkdef.h"
#include <memory>

namespace network {
	class tcpconnectionmgr;
	class tcpserver:public std::enable_shared_from_this<tcpserver>
	{
	public:
		explicit tcpserver(uint16_t port);
		~tcpserver();

		void run();
		void stop();
		void accept();
	private:
		ASIO::io_service m_io_ser;
		ASIOTCP::acceptor m_acceptor;
		// tcpserverlistener* m_listener;
		tcpconnectionmgr* m_connection_manager_ptr;
	};

	typedef std::shared_ptr<tcpserver> t_tcpser_ptr;
}
#endif // tcpserver_h__