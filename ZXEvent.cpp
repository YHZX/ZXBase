#include "ZXBase/ZXEvent.h"


namespace zx
{
	std::map<EventReciver*, EventReciver*>	EventReciver::s_EventReciverVector;

	EventReciver::EventReciver()
	{
		s_EventReciverVector[this] = this;
	}

	EventReciver::~EventReciver()
	{
		s_EventReciverVector.erase(s_EventReciverVector.find(this));
	}

	void EventSource::setEvent(EventPara * p)
	{
		for(std::map<EventReciver*, EventReciver*>::iterator itr = EventReciver::s_EventReciverVector.begin(); itr != EventReciver::s_EventReciverVector.end(); itr ++)
		{
			EventReciver* ev = itr->first;
			if(ev->onEvent(p) == EventStop) 
				return;
		}
	}

	void EventSource::SetEvent(EventPara * p)
	{
		static EventSource es;
		es.setEvent(p);
	}
};
