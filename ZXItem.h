#pragma once

#include "ZXBase/Export_ZXBase.h"
//////////////////////////////////////目前屏蔽了XML类的文件操作ZXXmlAchive，记得加上

namespace zx{

#pragma warning(disable:4251)

#define DECLARE_NEW_METHOD(classname) 
	//__forceinline void* operator new(size_t sizeInBytes, const char* file, int line)	{ void * p = malloc(sizeInBytes); ((classname*)(p))->outputinfo(true,#classname, file, line);return p;  }   \
	//__forceinline void  operator delete(void* ptr, const char* file, int line)			{ free(ptr); }   \
	//__forceinline void* operator new(size_t sizeInBytes)								{ void * p = malloc(sizeInBytes); ((classname*)(p))->outputinfo(true,#classname);return p;  }   \
	//__forceinline void  operator delete(void* ptr)										{ free(ptr); }   \
	//__forceinline void* operator new(size_t, void* ptr)									{ return ptr; }   \
	//__forceinline void  operator delete(void*, void*)									{ }   \
	//__forceinline void* operator new[](size_t sizeInBytes)								{ return malloc(sizeInBytes); }   \
	//__forceinline void  operator delete[](void* ptr)									{ free(ptr); }   \
	//__forceinline void* operator new[](size_t, void* ptr)   							{ return ptr; }   \
	//__forceinline void  operator delete[](void*, void*)									{ }

#define DeclareZXBase(classname, xmlnodename)\
	DECLARE_NEW_METHOD(classname);\
	static const char* static_class_name(){ return #classname; }\
	virtual const char* class_name(){ return #classname; }\
	static const char* static_xml_node_name(){ return #xmlnodename; }\
	virtual const char* xml_node_name(){ return #xmlnodename; }\
	virtual classname* clone(){ return new classname; }

	//class ZXXmlAchive;
	class ZXBase_API ZXItem
	{
		//Declare_List(ZXItem);
	public:								 //	静态成员函数，是类的一部分，不是对象的一部分，			
		static bool ValidItem(ZXItem* p);//类外调用公用的静态成员函数时要使用类名和与运算符:: 
	public:								 //其作用是为了能处静态数据成员
		ZXItem(); 

		DeclareZXBase(ZXItem, ZXItem);

		//void outputinfo(bool bnew, const char* classname, const char* file=0, int line = -1);
		int ref(); int unref(); int unref_nodelete(); int referenceCount();

		//virtual bool serial(ZXXmlAchive& ar){ return true; };

	protected:
		unsigned int	_flag;
		virtual ~ZXItem();
		int		_refCount;
	};

#define ZXItemValid			zx::ZXItem::ValidItem

	template<class T>
	class AutoPtr
	{
	public:
		AutoPtr() : _ptr(0) {}
		AutoPtr(T* ptr) : _ptr(ptr) { if (_ptr) _ptr->ref(); }
		AutoPtr(const AutoPtr& rp) : _ptr(rp._ptr) { if (_ptr) _ptr->ref(); }//复制构造函数：若用户未定义复制构造函数，则编译系统会自动提供
		~AutoPtr() { if (_ptr){try{_ptr->unref();}catch(...){}}  _ptr = 0; }//一个默认的复制构造函数其作用只是简单的复制类中每个数据成员

		AutoPtr& operator = (const AutoPtr& rp) { if (_ptr==rp._ptr) return *this; T* tmp_ptr = _ptr; _ptr = rp._ptr; if (_ptr) _ptr->ref(); if (tmp_ptr) tmp_ptr->unref(); return *this; }
		AutoPtr& operator = (T* ptr) { if (_ptr==ptr) return *this; T* tmp_ptr = _ptr; _ptr = ptr; if (_ptr) _ptr->ref(); if (tmp_ptr) tmp_ptr->unref(); return *this; }

		bool operator == (const AutoPtr& rp) const { return (_ptr==rp._ptr); }
		bool operator == (const T* ptr) const { return (_ptr==ptr); }
		bool operator != (const AutoPtr& rp) const { return (_ptr!=rp._ptr); }
		bool operator != (const T* ptr) const { return (_ptr!=ptr); }
		T& operator*() const { return *_ptr; }
		T* operator->() const { return _ptr; }
		T* get() const { return _ptr; }
		bool valid() const { return _ptr!=0; }
		T* release() { T* tmp=_ptr; if (_ptr) _ptr->unref_nodelete(); _ptr=0; return tmp; }
	private:
		T* _ptr;
	};

#define DeclareZXBasePtr(classname)\
	typedef classname * Ptr_##classname;\
	typedef zx::AutoPtr<classname> Ptr##classname

	DeclareZXBasePtr(ZXItem);

//#define DEBUG_ITEM_NEW(file,line)	new(file,line)
};
