#include "../../network/tcp/tcpserver.h"

int main(){
	network::tcpserver ser(9000, NULL);
	return 0;
}