#pragma once

#include <ZXBase/Export_ZXBase.h>

namespace zx{

#pragma warning(disable:4251)

	typedef __int64 Timer_t;

	class ZXBase_API Timer
	{
	public:
		Timer();
		virtual ~Timer(){};

		Timer_t tick() const;

		double delta_s( Timer_t t1, Timer_t t2 );

		DeclarePtrProperty(double,SecondPerTick);
	};

	class ZXBase_API TimerSpan
	{
	public:
		void begin(){ m_StartTime = m_Timer.tick(); };
		void end(){ m_EndTime = m_Timer.tick(); };

		inline double delta_s(){ return m_Timer.delta_s(m_StartTime,m_EndTime); };
		inline double delta_m(){ return delta_s()*1e3; };
		inline double delta_u(){ return delta_s()*1e6; };
		inline double delta_n(){ return delta_s()*1e9; };

		DeclarePtrProperty(Timer,Timer);
		DeclarePtrProperty(Timer_t,StartTime);
		DeclarePtrProperty(Timer_t,EndTime);
	};
};
