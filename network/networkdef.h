#ifndef networkdef_h__
#define networkdef_h__
#include <boost/asio.hpp>
#include <memory>

#define ASIO boost::asio
#define ASIOTCP boost::asio::ip::tcp

#define interface struct

interface tcpserverlistener {
	virtual ~tcpserverlistener() {};
	virtual void on_new_connection(const ASIOTCP::socket& sock) = 0;
};

typedef std::shared_ptr<ASIOTCP::socket> t_socket_ptr;

#endif // networkdef_h__
