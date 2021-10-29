#ifndef tcpconnection_h__
#define tcpconnection_h__
#include <memory>
#include "../networkdef.h"
namespace network {
	const int MaxLen = 4096;
	class tcpconnection :public std::enable_shared_from_this<tcpconnection> {
	public:
		explicit tcpconnection(ASIOTCP::socket sock_);
		~tcpconnection();

		void do_read();
		void do_write();

	private:
		ASIOTCP::socket m_sock;
		char m_data[MaxLen];
	};

	typedef std::shared_ptr<tcpconnection> tcpconnection_ptr;
};

#endif // tcpconnection_h__
