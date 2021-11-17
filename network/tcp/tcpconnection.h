#ifndef tcpconnection_h__
#define tcpconnection_h__
#include <memory>
#include "../networkdef.h"
namespace network {
	const int MaxLen = 4096;
	interface tcpconnectionlistener {
	public:
		virtual void on_recv(char* buffer, size_t length) = 0;
		virtual void on_write() = 0;
	};
	typedef std::shared_ptr<tcpconnectionlistener> tcpconnectionlistener_ptr;

	class tcpconnection :public std::enable_shared_from_this<tcpconnection> {
	public:
		explicit tcpconnection(t_socket_ptr _p_sock);
		~tcpconnection();

		void do_read();
		void do_write();

		void on_recv(char* buffer, size_t length);

	private:
		t_socket_ptr m_p_sock;
		char m_data[MaxLen];
		tcpconnectionlistener_ptr m_listener;
	};

	typedef std::shared_ptr<tcpconnection> tcpconnection_ptr;
};

#endif // tcpconnection_h__
