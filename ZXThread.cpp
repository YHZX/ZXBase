#include <process.h>
#include "ZXBase/ZXThread.h"
//#include "ZXBase/ZXLog.h"

namespace zx{
	ZXMutex::ZXMutex()
	{
		InitializeCriticalSection( &_cs );
	}
	void ZXMutex::lock()
	{
		EnterCriticalSection( &_cs );
	}
	void ZXMutex::unlock()
	{
		LeaveCriticalSection( &_cs );
	}

	ZXMutex::~ZXMutex()
	{
		DeleteCriticalSection( &_cs );
	}

#pragma warning(disable:4355)
	ZXThread::ZXThread(ThreadFunction fun)
		: m_Parameter(this, 0)
		, m_Function(fun)
		, m_Status(None)
		, m_id(0)
	{
		Register_List_Variation(ZXThread);
		m_BeginEvent = ::CreateEvent(0,TRUE,TRUE,0);//人工手动复原，初始状态有信号
		m_EndEvent = ::CreateEvent(0,TRUE,TRUE,0);
		::ResetEvent(m_BeginEvent);
		::ResetEvent(m_EndEvent);
		m_SelfEvent = 0;
	}
#pragma warning(default:4355)

	ZXThread::~ZXThread()
	{
		close_thread(0);
		::CloseHandle(m_BeginEvent);
		::CloseHandle(m_EndEvent);
		UnRegister_List_Variation(ZXThread);
	}

	bool ZXThread::begin_thread(void* p, bool pause)
	{
		m_Parameter.setPtr(p);
		setStatus(pause?Pause:Run);
		m_id = ::CreateThread(0, 0, m_Function, &m_Parameter, pause?CREATE_SUSPENDED|THREAD_TERMINATE:THREAD_TERMINATE, 0);
		return m_id != 0;
	}

	bool ZXThread::pause_thread()
	{
		if(m_id == 0)
			return false;

		if(m_Status == Pause)
			return true;
		if(m_Status == None)
			return false;

		SuspendThread(m_id);

		m_Status = Pause;

		return true;
	}

	bool ZXThread::resume_thread()
	{
		if(m_id == 0)
			return false;

		if(m_Status == Run)
			return true;

		ResumeThread(m_id);//若为None,则也需唤醒

		m_Status = Run;

		return true;
	}

	bool ZXThread::close_thread(unsigned int code)
	{
		if(m_id == 0)
			return true;

		if(m_Status != None)
		{
			TerminateThread(m_id, code);
			::ResetEvent(m_EndEvent);
			::SetEvent(m_EndEvent);
			setStatus(None);
		}
		CloseHandle(m_id);
		m_id = 0;
		return true;
	}
};
