#include <ZXBase/ZXLog.h>

namespace zx{

	ZXLogItem::ZXLogItem()
	{
		::GetLocalTime(m_Time);
		setID(LIID);
		setLevel(Normal);
	}
	ZXLogItem::ZXLogItem(const char* log)
	{
		::GetLocalTime(m_Time);
		m_Content = log;
		setID(LIID);
		setLevel(Normal);
	}
	ZXLogItem::ZXLogItem(unsigned int id, ZXLogItem::Level level, const char* log)
	{
		::GetLocalTime(m_Time);
		m_Content = log;
		setID(id);
		setLevel(level);
	}

	class ZXLogItemRegisterDescribute
	{
	public:
		ZXLogItemRegisterDescribute()
		{
			ZXLogItem::GetIDDescribute()[ZXLogItem::LINone] = "未知来源";
			ZXLogItem::GetIDDescribute()[ZXLogItem::LIID] = "系统";
		}
	}s_ZXLogItemRegisterDescribute;


	ZXLog::ZXLog(bool enable)
	{
		Register_List_Variation(ZXLog);
		setEnable(enable);
	}
	ZXLog::~ZXLog()
	{
		UnRegister_List_Variation(ZXLog);
	}
	void ZXLog::addLog(unsigned int id, ZXLogItem::Level level, const char* format, ...)
	{
		try{
			char s_log_buf[1024];
			memset(s_log_buf, 0, 1024);
			MarcoBuffer(format, s_log_buf);
			PtrZXLogItem item = new ZXLogItem(id,level,s_log_buf);
			for(mapZXLog::iterator itr = get_map_class_object().begin(); itr != get_map_class_object().end(); itr ++)
			{//定义基类ZXLOG的关联容器迭代器指针，get_map_class_object.begin()获得的ZXLlog派生类（如TraceLog)的迭代器
				if(itr->second->getEnable())
					itr->second->log(item.get());//则此处调用的就是相应ZXLog派生类的log函数(如TraceLog::log()),
				
			}//此方法可以获得任何一个由ZXLog重载的类
		}catch(...)
		{

		}
	}

	void ZXLog::addLog(const char* format, ...)
	{
		try{
			char s_log_buf[1024];
			memset(s_log_buf, 0, 1024);
			MarcoBuffer(format, s_log_buf);
			PtrZXLogItem item = new ZXLogItem(s_log_buf);
			for(mapZXLog::iterator itr = get_map_class_object().begin(); itr != get_map_class_object().end(); itr ++)
			{
				if(itr->second->getEnable())
					itr->second->log(item.get());
			}
		}catch(...)
		{

		}
	}

	class TraceLog : public ZXLog
	{
	public:
		TraceLog():ZXLog(false){}
		virtual void log(ZXLogItem* item)
		{
			char s_log_buf[1024];
			sprintf_s(s_log_buf, "%04d-%02d-%02d %02d:%02d:%02d %03d | %s\n",
				item->getTime().wYear, item->getTime().wMonth, item->getTime().wDay,
				item->getTime().wHour, item->getTime().wMinute, item->getTime().wSecond,
				item->getTime().wMilliseconds, item->getContent().c_str());
			::OutputDebugStringA(s_log_buf);//s_log_buf为栈空间上的内存，不能使用返回值那个等函数结束后即自动释放
		}
	}__Log_Trace;
};
