#include "evser.h"

evser::evser()
{
	m_tcpser = std::make_shared<network::tcpserver>(4000);
	
}

void evser::on_recv(char* buffer, size_t length)
{

}

void evser::on_write()
{

}
