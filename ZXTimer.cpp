#include "ZXBase/ZXTimer.h"

#include <sys/types.h>
#include <fcntl.h>
#include <windows.h>
#include <winbase.h>

namespace zx
{
	Timer::Timer()
	{
		LARGE_INTEGER frequency;//LARGE_INTEGER 64位的共同体
		if(::QueryPerformanceFrequency(&frequency))//获取当前的性能，无法获取时返回0
			m_SecondPerTick = 1.0/(double)frequency.QuadPart;
		else
			m_SecondPerTick = 1.0;
	}

	Timer_t Timer::tick() const
	{
		LARGE_INTEGER qpc;
		if (::QueryPerformanceCounter(&qpc))
			return qpc.QuadPart;
		else
			return 0;
	}

	double Timer::delta_s( Timer_t t1, Timer_t t2 )
	{ 
		return (double)(t2 - t1)*m_SecondPerTick; 
	}
};
