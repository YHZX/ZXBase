#pragma once

//#include "ZXBase/ZXAchive.h"
//#include "ZXBase/ZXFactory.h"
#include "ZXBase/ZXItem.h"
#include <vector>
#include "ZXBase/ZXThread.h"
namespace zx{

template<class T>
class ZXVector : public std::vector<AutoPtr<T> >, public ZXItem
{
public:
	ZXMutex		m_mutex;
	typedef T Type;
	DeclareZXBase(ZXVector, ZXVector);

	void erase(T* item);
	//{
	//	ZXLock	lock(m_mutex);
	//	for(unsigned int i = 0; i < size(); i ++)
	//	{
	//		if((*this)[i].get() == item)
	//		{
	//			std::vector<AutoPtr<T> >::erase(begin()+i);
	//			return;
	//		}
	//	}
	//}

	void clear()
	{
		ZXLock	lock(m_mutex);
		std::vector<AutoPtr<T> >::clear();
	}

	void push_back(AutoPtr<T> node)
	{
		ZXLock	lock(m_mutex);
		std::vector<AutoPtr<T> >::push_back(node);
	}

	//virtual bool serial(ZXXmlAchive& ar)
	//{
	//	if(ar.flag() == ZXXmlAchive::read)
	//	{
	//		ZXXmlNode* child = ar.handle()->FirstChildElement();
	//		while(child)
	//		{
	//			PtrZXItem item = CreateZXClass(child->Value());
	//			ZXXmlInAchive read(child);
	//			if(!item->serial(read))
	//				return false;
	//			T* p = dynamic_cast<T*>(item.get());
	//			if(p) push_back(p);
	//			child = child->NextSiblingElement();
	//		}
	//	}
	//	else
	//	{
	//		for(unsigned int i = 0; i < size(); i ++)
	//		{
	//			Ptr_ZXItem item = (*this)[i].get();
	//			ZXXmlNode* child = new ZXXmlNode(T::static_xml_node_name());
	//			ZXXmlOutAchive write(child);
	//			if(!item->serial(write))
	//				return false;

	//			ar.handle()->LinkEndChild(child);
	//		}
	//	}
	//	return true;
	//}
};

};
