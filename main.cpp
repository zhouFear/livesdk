#include "network/tcp/tcpserver.h"

int main() {
	network::t_tcpser_ptr ser = std::make_shared<network::tcpserver>(2001);
	ser->run();
}