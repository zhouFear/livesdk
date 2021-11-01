#include "../../network/tcp/tcpserver.h"

int main(){
	network::tcpserver ser(2001);
	ser.run();
	return 0;
}