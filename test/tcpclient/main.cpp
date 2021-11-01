#include <boost/asio.hpp>
#include <stdio.h>

using namespace boost::asio;
const static int MaxLen = 4096;
int main() {
	io_service service;
	ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 2001);
	ip::tcp::socket sock(service);
	sock.connect(ep);
	sock.write_some(buffer("hello world!"));
	return 0;
}