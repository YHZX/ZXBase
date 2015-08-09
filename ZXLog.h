#pragma once

#include "ZXBase/ZXItem.h"
//#include "ZXBase/ZXVector.h"
#include <list>
#include "ZXBase/ZXThread.h"

#include <windows.h>

namespace zx{

	class ZXBase_API ZXTime : public SYSTEMTIME
	{
	public:

		template<typename T>
		int compare(T v1, T v2){
			if(v1 < v2) return -1;
			else if(v1 == v2) return 0;
			return 1;
		}

		virtual ZXTime& operator = (SYSTEMTIME& time)
		{
			wYear=time.wYear,wMonth=time.wMonth,wDay=time.wDay;
			wHour=time.wHour,wMinute=time.wMinute,wSecond=time.wSecond;
			wDayOfWeek=time.wDayOfWeek,wMilliseconds=time.wMilliseconds;
			return *this;
		}
		virtual ZXTime& operator = (ZXTime& time)
		{
			wYear=time.wYear,wMonth=time.wMonth,wDay=time.wDay;
			wHour=time.wHour,wMinute=time.wMinute,wSecond=time.wSecond;
			wDayOfWeek=time.wDayOfWeek,wMilliseconds=time.wMilliseconds;
			return *this;
		}

		virtual bool operator == (ZXTime& time)
		{
			return compare(wYear,time.wYear)==0
				&& compare(wMonth,time.wMonth)==0
				&& compare(wDay,time.wDay)==0
				&& compare(wHour,time.wHour)==0
				&& compare(wMinute,time.wMinute)==0
				&& compare(wSecond,time.wSecond)==0
				&& compare(wMilliseconds,time.wMilliseconds)==0;
		}

		virtual bool operator != (ZXTime& time)
		{
			return !( (*this)==time );
		}

		virtual bool operator < (ZXTime& time)
		{
			switch(compare(wYear,time.wYear))
			{
			case -1:
				return true;
			case 0:
				{
					switch(compare(wMonth,time.wMonth))
					{
					case -1:
						return true;
					case 0:
						{
							switch(compare(wDay,time.wDay))
							{
							case -1:
								return true;
							case 0:
								{
									switch(compare(wHour,time.wHour))
									{
									case -1:
										return true;
									case 0:
										{
											switch(compare(wMinute,time.wMinute))
											{
											case -1:
												return true;
											case 0:
												{
													switch(compare(wSecond,time.wSecond))
													{
													case -1:
														return true;
													case 0:
														{
															switch(compare(wMilliseconds,time.wMilliseconds))
															{
															case -1:
																return true;
															case 0:
															case 1:
															default:
																return false;
															}
														}
													case 1:
													default:
														return false;
													}
												}
											case 1:
											default:
												return false;
											}
										}
									case 1:
									default:
										return false;
									}
								}
							case 1:
							default:
								return false;
							}
						}
					case 1:
					default:
						return false;
					}
				}
			case 1:
			default:
				return false;
			}
		}
		virtual operator LPSYSTEMTIME ()
		{
			return this;
		}
	};

	class ZXBase_API ZXLogItem : public ZXItem
	{
	public:
		enum LogItemID{ LINone = 0, LIID };
		enum Level{ Normal, Warning, Error };
	public:
		typedef std::map<unsigned int, ZXString>	IDDescribute;
		static IDDescribute& GetIDDescribute(){ static IDDescribute s_IDDescribute; return s_IDDescribute; };
		static ZXString GetIDDescribute(unsigned int id){
			IDDescribute& s = GetIDDescribute();//调用后类中就有一个static IDDescribute
			IDDescribute::iterator itr = s.find(id);
			if(itr != s.end())
				return itr->second;
			return GetIDDescribute(LINone);
		};
	public:

		ZXLogItem(); 
		ZXLogItem(const char* log); 
		ZXLogItem(unsigned int id, ZXLogItem::Level level, const char* log); //对枚举变量的使用

		DeclareZXBase(ZXLogItem, LogItem);


		DeclareProperty(ZXTime, Time);
		DeclareProperty(ZXString, Content);
		DeclareProperty(unsigned int, ID);
		DeclareProperty(Level, Level);
		DeclareProperty(ZXString, ExtString);
	protected:
		virtual ~ZXLogItem(){};
	};

	DeclareZXBasePtr(ZXLogItem);

	class ZXBase_API ZXLog
	{
		Declare_List(ZXLog);
	public:
		static void addLog(unsigned int id, ZXLogItem::Level level, const char* format, ...);
		static void addLog(const char* format, ...);
	public:
		ZXLog(bool enable=true);
		virtual ~ZXLog();

		virtual void log(ZXLogItem*) = 0;

		DeclarePtrProperty(bool,Enable);
	};
#define AddLog			zx::ZXLog::addLog
};
