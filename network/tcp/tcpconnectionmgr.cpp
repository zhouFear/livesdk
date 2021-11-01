#include "tcpconnectionmgr.h"

network::tcpconnectionmgr::tcpconnectionmgr()
{

}

void network::tcpconnectionmgr::start(tcpconnection_ptr _conn)
{
	m_connectionset.insert(_conn);
	_conn->do_read();
}
