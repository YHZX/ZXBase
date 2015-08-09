#pragma once 

#include <ZXBase/Export_ZXBase.h>
#include <windows.h>

namespace zx{

	typedef DWORD (__stdcall *ThreadFunction)(void*);
	typedef void * VoidPtr;

	class ZXBase_API ZXMutex 
	{
	public:
		ZXMutex();
		virtual ~ZXMutex();

		void lock();
		void unlock();

		CRITICAL_SECTION	_cs;
	};
	class ZXBase_API ZXLock
	{
	public:
		ZXLock(ZXMutex& m):_m(m){m.lock();};
		virtual ~ZXLock(){_m.unlock();};

		ZXMutex& _m;
	};
	/*
	线程管理类
	*/
	class ZXBase_API ZXThread
	{
		Declare_List(ZXThread);
	public:
		enum Status{
			None = 0,	//没有启动或者已经结束
			Run,		//运行中
			Pause,		//被挂起
		};
		class ZXBase_API Parameter
		{
		public:
			Parameter(ZXThread* thread,VoidPtr ptr = 0)
				: m_Thread(thread)
				, m_Ptr(ptr)
			{};
			DeclarePtrProperty(ZXThread*, Thread);
			DeclareProperty(VoidPtr, Ptr);
			DeclareProperty(ZXString, FunctionName);
		};
	public:
		ZXThread(ThreadFunction fun);
		virtual ~ZXThread();

		virtual bool begin_thread(void*, bool pause = false);
		virtual bool pause_thread();
		virtual bool resume_thread();
		virtual bool close_thread(unsigned int code);
		virtual bool is_runing(){ return m_Status != None;}
		virtual bool is_pause(){ return m_Status == Pause;}

		virtual bool CreateSelfEvent(){m_SelfEvent=::CreateEvent(0,TRUE,TRUE,0);::ResetEvent(m_SelfEvent);return true;}
		virtual void DestroySelfEvent(){::CloseHandle(m_SelfEvent); m_SelfEvent = 0;}
		virtual void SetSelfEvent(){::SetEvent(m_SelfEvent);}
		virtual void ResetSelfEvent(){::ResetEvent(m_SelfEvent);}

		DeclareProperty(ThreadFunction, Function);
		DeclareProperty(Status, Status);
		DeclareProperty(Parameter, Parameter);
		DeclareProperty(HANDLE, id);
		DeclareProperty(ZXString, Name);
		DeclareProperty(ZXString, Describute);
		DeclareProperty(HANDLE, BeginEvent);
		DeclareProperty(HANDLE, EndEvent);
		DeclareProperty(HANDLE, SelfEvent);
	};

#define BeginDeclareThread(function_name)\
	DWORD __stdcall function_name(void* para)\
	{\
	zx::ZXThread::Parameter* tPara = (zx::ZXThread::Parameter*)para;\
	tPara->setFunctionName(#function_name);\
	zx::ZXThread* pThread = tPara->getThread();\
	pThread->setStatus(zx::ZXThread::Run);\
	::ResetEvent(pThread->getBeginEvent());\
	::SetEvent(pThread->getBeginEvent());\

#define ZXExitThread(code)\
	{\
	::ResetEvent(pThread->getEndEvent());\
	::SetEvent(pThread->getEndEvent());\
	pThread->setStatus(zx::ZXThread::None);\
	::ExitThread(code);\
	}

#define EndDeclareThread(function_name) \
	pThread->setStatus(zx::ZXThread::None);\
	::ResetEvent(pThread->getEndEvent());\
	::SetEvent(pThread->getEndEvent());\
	::ExitThread(0);\
	return 0;\
	}

};
