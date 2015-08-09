#pragma once

#include "ZXBase/ZXItem.h"
#include <vector>
#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <common.h>

namespace zx{

	typedef SOCKET SocketID;

	enum{
		ZX_SOCKET_ERROR_BEGIN = 0x01000000,
		ZX_SOCKET_UNKNOWN_ERROR = ZX_SOCKET_ERROR_BEGIN,
		ZX_SOCKET_RECV_NODATA,
		ZX_SOCKET_CONNECTION_REFUSED,
		ZX_SOCKET_SHUT_DOWN,
		ZX_SOCKET_CONNECTION_RESET_BY_PEER,
		ZX_SOCKET_SOCKET_NOT_CONNECTED,
		ZX_SOCKET_SOCKET_ALREADY_CONNECTED,
		ZX_SOCKET_CONNECTION_TIMED_OUT,
	};

	class ZXBase_API ZXSocket : public ZXItem
	{
	public:
		DeclareZXBase(ZXSocket, ZXSocket);

		ZXSocket();
		ZXSocket(SocketID id);
		ZXSocket(int af, int type, int protocol);

		bool vaild(){ return m_SocketID != INVALID_SOCKET; };

		virtual bool bind(ZXString ip, u_short port);
		virtual bool listen(int backlog);
		virtual bool connect(ZXString ip, u_short port);
		virtual AutoPtr<ZXSocket> accept(ZXString& ip, u_short& port);
		virtual AutoPtr<ZXSocket> accept();

		bool send(char* buff, unsigned int size);
		bool recv(char* buff, unsigned int size);
		bool send(const char* buff, unsigned int size){ return send(const_cast<char*>(buff), size); };
		bool recv(const char* buff, unsigned int size){ return recv(const_cast<char*>(buff), size); };

		bool send(ZXString& value);
		bool recv(ZXString& value);

		bool send(NetCommand& value);
		bool recv(NetCommand& value);

		template<class T>
		bool send(T& value){ return send((char*)&value, sizeof(T)); };
		template<class T>
		bool recv(T& value){ return recv((char*)&value, sizeof(T)); };


		bool send_file(ZXString file);
		bool recv_file(ZXString file);

		DeclarePtrProperty(SocketID,SocketID);

		void setLastError(int error);
	protected:
		virtual ~ZXSocket();// 虚析构函数，继承各类的析构一层层从派生类到基类析构
	};

	DeclareZXBasePtr(ZXSocket);

	class ZXBase_API TCPSocket : public ZXSocket
	{
	public:
		DeclareZXBase(TCPSocket, TCPSocket);
		TCPSocket():ZXSocket(AF_INET,SOCK_STREAM,IPPROTO_TCP){};
		TCPSocket(SocketID id):ZXSocket(id){};
	protected:
		virtual ~TCPSocket(){};
	};

	DeclareZXBasePtr(TCPSocket);
	extern bool ZXBase_API get_local_ip(std::vector<ZXString>&);
	extern ZXString ZXBase_API get_local_ip();
};
