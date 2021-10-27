#ifndef tcpserver_h__
#define tcpserver_h__
#include "../networkdef.h"
#include <boost/enable_shared_from_this.hpp>

namespace network {

	interface tcpserverlistener {
		virtual ~tcpserverlistener() {};
		virtual void on_new_connection() = 0;
	};

	class tcpserver:public boost::enable_shared_from_this<tcpserver>
	{
	public:
		explicit tcpserver(uint16_t port, tcpserverlistener* listener);
		~tcpserver();

		void run();
		void stop();
		void accept();
	private:
		ASIO::io_service m_io_ser;
		ASIO::ip::tcp::acceptor m_acceptor;
		tcpserverlistener* m_listener;

	};
}
#endif // tcpserver_h__