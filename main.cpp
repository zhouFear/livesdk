#include "network/tcp/tcpserver.h"

int main() {
	network::tcpserver ser(80000, NULL);
	ser.run();
}