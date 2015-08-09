#include "ZXBase/ZXItem.h"
#include "ZXBase/ZXThread.h"
/////////////////////////////////////目前屏蔽了ZXThread，记得加上

namespace zx{

	//定义一个全局保护对象
	ZXMutex* s_item_mutex = 0;
	class ZXBase_API ZXItemLock
	{
	public:
		ZXItemLock()
		{
			if(s_item_mutex == 0)
			{
				s_item_mutex = new ZXMutex;
			}
			s_item_mutex->lock();
		};
		virtual ~ZXItemLock()
		{
			s_item_mutex->unlock();
		};
	};

	typedef std::map<ZXItem*,ZXItem*> mapZXItem;
	mapZXItem& get_map_class_object()
	{ 
		static mapZXItem* s_list_ZXItem = new mapZXItem; 
		return *s_list_ZXItem; 
	}
	bool ZXItem::ValidItem(ZXItem* p)
	{
		if((unsigned int)p == 0xfeeefeee) return false;
		//mapZXItem& item_list = get_map_class_object();
		//mapZXItem::iterator itr = item_list.find(p);
		//if(itr == item_list.end()) return false;
		if(p->_flag == 0xfeeefeee) return false;
		return true;
	}

	ZXItem::ZXItem()
	{
		//ZXItemLock lock;
		_refCount=0;
		//mapZXItem& item_list = get_map_class_object();
		//item_list[this] = this;
		_flag = 0;
	}

	ZXItem::~ZXItem()
	{
		//ZXItemLock lock;
		//mapZXItem& item_list = get_map_class_object();
		//mapZXItem::iterator itr = item_list.find(this);
		//if(itr != item_list.end()) item_list.erase(itr);
	}

	int ZXItem::ref()
	{
		_refCount++;
		return _refCount;
	}

	int ZXItem::unref()
	{
		_refCount--; 
		if (_refCount<=0)
		{
			//outputinfo(false, class_name());
			delete this; 
			return 0; 
		}
		return _refCount; 
	}

	int ZXItem::unref_nodelete()
	{ 
		_refCount--; 
		return _refCount; 
	}

	int ZXItem::referenceCount()
	{ 
		return _refCount; 
	}

};
