#pragma once

#include "ZXBase/Export_ZXBase.h"

namespace zx{

	class ZXBase_API EventPara
	{
	public:
		EventPara(){};
		virtual ~EventPara(){};
	};

	enum EventResult{
		EventContinue = 0,
		EventStop,
	};

	class ZXBase_API EventSource
	{
	public:
		static void SetEvent(EventPara*);
	public:
		virtual void setEvent(EventPara*);
	};

#pragma warning(disable:4251)
	class ZXBase_API EventReciver
	{
		friend public EventSource;
	private:
		static std::map<EventReciver*, EventReciver*>	s_EventReciverVector;
	public:
		EventReciver();
		virtual ~EventReciver();
		virtual EventResult onEvent(EventPara*){ return EventContinue; };
	};
#pragma warning(default:4251)
};
