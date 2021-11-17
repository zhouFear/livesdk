#ifndef evser_h__
#define evser_h__
#include "../../network/tcp/tcpserver.h"
#include "../../network/tcp/tcpconnection.h"

using namespace network;

class evser : public tcpconnectionlistener
{
public:
	evser();
	~evser();

	void on_recv(void* buffer, size_t length);
private:
	t_tcpser_ptr m_tcpser;
};

#endif // evser_h__
