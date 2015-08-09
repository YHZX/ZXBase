#include "ZXBase/ZXSocket.h"
#include "ZXBase/ZXError.h"
#include "ZXBase/ZXLog.h"

#include <fstream>
#include <Ws2tcpip.h>


namespace zx
{
	class SocketInit{
	public:
		SocketInit()
		{
			WSADATA wsaData;
			WSAStartup( MAKEWORD(2, 2), &wsaData );
			ZXRegisterErrorCode(ZX_SOCKET_RECV_NODATA,"接收到0字节数据");
			ZXRegisterErrorCode(ZX_SOCKET_UNKNOWN_ERROR,"网络连接未知错误");
			ZXRegisterErrorCode(ZX_SOCKET_CONNECTION_REFUSED,"连接被拒绝，远程机器没有开机");
			ZXRegisterErrorCode(ZX_SOCKET_SHUT_DOWN,"远程机器被关闭");
			ZXRegisterErrorCode(ZX_SOCKET_CONNECTION_RESET_BY_PEER,"连接被远程机器重置");
			ZXRegisterErrorCode(ZX_SOCKET_SOCKET_NOT_CONNECTED,"被远程机器没有连接");
			ZXRegisterErrorCode(ZX_SOCKET_SOCKET_ALREADY_CONNECTED,"当前连接已经连接");
			ZXRegisterErrorCode(ZX_SOCKET_CONNECTION_TIMED_OUT,"连接超时");
		};
		virtual ~SocketInit()
		{
			WSACleanup( );
		}
	};

	void ZXSocket::setLastError(int error)
	{
		//AddLog("Socket Error = %d", error);
		switch(error)
		{
		case WSAECONNREFUSED:
			ZXSetLastError(ZX_SOCKET_CONNECTION_REFUSED);
			break;
		case WSAESHUTDOWN:
			ZXSetLastError(ZX_SOCKET_SHUT_DOWN);
			break;
		case WSAECONNRESET:
			ZXSetLastError(ZX_SOCKET_CONNECTION_RESET_BY_PEER);
			break;
		case WSAENOTCONN:
			ZXSetLastError(ZX_SOCKET_SOCKET_NOT_CONNECTED);
			break;
		case WSAEISCONN:
			ZXSetLastError(ZX_SOCKET_SOCKET_ALREADY_CONNECTED);
			break;
		case WSAETIMEDOUT:
			ZXSetLastError(ZX_SOCKET_CONNECTION_TIMED_OUT);
			break;
		default:
			ZXSetLastError(ZX_SOCKET_UNKNOWN_ERROR);
			break;
		}
	}

	ZXSocket::ZXSocket()
	{
		m_SocketID = INVALID_SOCKET;
	}

	ZXSocket::ZXSocket(SOCKET socket)
	{
		m_SocketID = socket;
	}

	ZXSocket::ZXSocket(int af, int type, int protocol)
	{
		static SocketInit s_socket_init;
		m_SocketID = ::socket(af, type, protocol);
	}

	ZXSocket::~ZXSocket()
	{
		if(vaild())
		{
// 			BB= ::shutdown(m_SocketID, SD_BOTH);
			::closesocket(m_SocketID);
		}
	}

	bool ZXSocket::bind(ZXString ip, u_short port)
	{
		SOCKADDR_IN addrLocal;
		addrLocal.sin_family = AF_INET;
		addrLocal.sin_port = htons(port);
		addrLocal.sin_addr.s_addr = inet_addr(ip.c_str());//c_str返回const char*类型，由string到标准的C字符串

		if(::bind(m_SocketID, (SOCKADDR *)&addrLocal, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
		{
			setLastError(WSAGetLastError());
			return false;
		}

		return true;
	}

	bool ZXSocket::listen(int backlog)
	{
		if(::listen(m_SocketID, backlog) == SOCKET_ERROR)
		{
			setLastError(WSAGetLastError());
			return false;
		}

		return true;
	}

	bool ZXSocket::connect(ZXString ip, u_short port)
	{
		SOCKADDR_IN addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr(ip.c_str());

		if(::connect(m_SocketID, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
		{
			setLastError(WSAGetLastError());
			return false;
		}

		return true;
	}

	AutoPtr<ZXSocket> ZXSocket::accept()
	{
		SOCKADDR_IN sai;
		int iAddrlength = sizeof(SOCKADDR_IN);
		SOCKET s = ::accept(m_SocketID, (SOCKADDR *)&sai, &iAddrlength);
		return new ZXSocket(s);
	}

	AutoPtr<ZXSocket> ZXSocket::accept(std::string& ip, u_short& port)
	{
		SOCKADDR_IN sai;
		int iAddrlength = sizeof(SOCKADDR_IN);
		SOCKET s = ::accept(m_SocketID, (SOCKADDR *)&sai, &iAddrlength);
		ip = inet_ntoa(sai.sin_addr);
		port = sai.sin_port;
		return new ZXSocket(s);
	}

	bool ZXSocket::send(char* buff, unsigned int size)
	{
		int i_send = 0, i_leave = size;
		while(true)
		{
			i_send = ::send(m_SocketID, buff+size-i_leave, i_leave, 0);

			if(i_send == SOCKET_ERROR)
			{
				setLastError(WSAGetLastError());
				return false;
			}

			if(i_send == 0)
			{
				ZXSetLastError(ZX_SOCKET_RECV_NODATA);
				return false;
			}

			i_leave -= i_send;

			if(i_leave == 0)
			{
				return true;
			}
		}

		return false;
		//if(::send(m_SocketID, buff, size, 0) == SOCKET_ERROR)
		//{
		//	setLastError(WSAGetLastError());
		//	return false;
		//}
		//return true;
	}

	bool ZXSocket::recv(char* buff, unsigned int size)
	{
		int i_recv = 0, i_leave = size;
		while(true)
		{
			i_recv = ::recv(m_SocketID, buff+size-i_leave, i_leave, 0);
			if(i_recv == SOCKET_ERROR)
			{
				if(WSAETIMEDOUT == WSAGetLastError())
				{
					continue;
				}
				setLastError(WSAGetLastError());
				return false;
			}

			if(i_recv == 0)
			{
				ZXSetLastError(ZX_SOCKET_RECV_NODATA);
				return false;
			}

			i_leave -= i_recv;

			if(i_leave == 0)
			{
				return true;
			}
		}

		return false;
	}

	bool ZXSocket::send(ZXString& value)
	{
		size_t size = value.length();
		if(!send<size_t>(size)) return false;
#pragma warning(disable:4267)
		return send(value.c_str(), size);
#pragma warning(default:4267)
	}

	bool ZXSocket::recv(ZXString& value)
	{
		size_t size;
		if(!recv<size_t>(size)) return false;
		value.resize(size);
#pragma warning(disable:4267)
		return recv(value.c_str(), size);//string 的标准C字符串形式
#pragma warning(default:4267)
	}

	bool ZXSocket::send(NetCommand& value)
	{
		if(!send<unsigned int>(value.cmd)) return false;
		if(!send<unsigned int>(value.sub_cmd)) return false;
		if(!send(value.para, NetCommandParaSize)) return false;
		size_t size = value.string_list.size();
		if(!send<size_t>(size)) return false;
		for(size_t i = 0; i < size; i ++)
		{
			if(!send(value.string_list[i]))
				return false;
		}
		return true;
	}

	bool ZXSocket::recv(NetCommand& value)
	{
		if(!recv<unsigned int>(value.cmd)) return false;
		if(!recv<unsigned int>(value.sub_cmd)) return false;
		if(!recv(value.para, NetCommandParaSize)) return false;
		value.string_list.clear();
		size_t size;
		if(!recv<size_t>(size)) return false;
		for(size_t i = 0; i < size; i ++)
		{
			std::string str;
			if(!recv(str))
				return false;
			value.string_list.push_back(str);
		}
		return true;
	}

#define FileBufferSize		65536

	bool ZXSocket::send_file(ZXString file)
	{
		std::ifstream fp(file.c_str());
		if(!fp.is_open())
			return false;

		fp.seekg(0, std::ios::end);
		size_t size = fp.tellg();//能表示文件长度
		if(!send<size_t>(size))
			return false;

		bool bRet;
		recv<bool>(bRet);

		fp.seekg(0, std::ios::beg);
		char buffer[FileBufferSize];
		while(size > 0)
		{
#pragma warning(disable:4267)
			size_t read = size > FileBufferSize ? FileBufferSize : size;
			fp.read(buffer, read);
			if(!send(buffer, read))
				return false;
			size -= read;
			recv(bRet);
#pragma warning(default:4267)
		}
		return true;
	}

	bool ZXSocket::recv_file(ZXString file)
	{
		std::ofstream fp(file.c_str());
		if(!fp.is_open())
			return false;

		size_t size;
		if(!recv<size_t>(size))
			return false;

		bool bRet;
		send<bool>(bRet);

		char buffer[FileBufferSize];
		while(size > 0)
		{
#pragma warning(disable:4267)
			size_t read = size > FileBufferSize ? FileBufferSize : size;
			if(!recv(buffer, read))
				return false;
			fp.write(buffer, read);
			size -= read;
			send(bRet);
#pragma warning(default:4267)
		}
		return true;
	}

	bool get_local_ip(std::vector<ZXString>& ip_list)
	{
		AutoPtr<TCPSocket> init = new TCPSocket;
		char szName[MAX_PATH];
		memset(szName, 0, MAX_PATH);
		if(::gethostname(szName, MAX_PATH) == SOCKET_ERROR)
		{
			int error = WSAGetLastError(); 
			return false;
		}
		addrinfo* pAddrInfo = NULL;
		::getaddrinfo(szName, "", NULL, &pAddrInfo);
		while( pAddrInfo != NULL )
		{
			std::string ip = inet_ntoa(((struct sockaddr_in *)pAddrInfo->ai_addr)->sin_addr);
			if(ip!="0.0.0.0") ip_list.push_back(ip);
			pAddrInfo = pAddrInfo->ai_next;
		}
		return ip_list.size() != 0;
	}

	ZXString get_local_ip()
	{
		std::vector<ZXString> ip_list;
		if(!get_local_ip(ip_list))
			return "";

		return ip_list[0];
	}

};
