#include <boost/asio/io_service.hpp>
#include <boost/asio/socket_base.hpp>
#include <boost/asio/ip/tcp.hpp>

#define ASIO boost::asio

int main() {
	ASIO::io_service ser;
	ASIO::ip::tcp::socket sock(ser);
	ser.run();
	return 0;
}