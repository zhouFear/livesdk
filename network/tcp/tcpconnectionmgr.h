#ifndef tcpconnectionmgr_h__
#define tcpconnectionmgr_h__
#include <set>
#include "tcpconnection.h"

namespace network {
	class tcpconnectionmgr {
	public:
		explicit tcpconnectionmgr();
		~tcpconnectionmgr();

		void start(tcpconnection_ptr _conn);
	private:
		std::set<tcpconnection_ptr> m_connectionset;
	};
};
#endif // tcpconnectionmgr_h__
