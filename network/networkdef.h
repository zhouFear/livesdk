#ifndef networkdef_h__
#define networkdef_h__
#include <boost/asio.hpp>

#define ASIO boost::asio
#define ASIOTCP boost::asio::ip::tcp

#define interface struct

interface tcpserverlistener {
	virtual ~tcpserverlistener() {};
	virtual void on_new_connection(const ASIOTCP::socket& sock) = 0;
};

#endif // networkdef_h__
