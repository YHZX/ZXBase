#pragma once

#ifdef _ZXBase
#	define ZXBase_API	__declspec(dllexport)
#else
#	define ZXBase_API	__declspec(dllimport)
#	ifdef _DEBUG
#		pragma comment(lib,"../lib/ZXBase_d.lib")
#	else
#		pragma comment(lib,"../lib/ZXBase.lib")
#	endif
#endif

#include <string>
#include <map>

namespace zx{
	typedef std::string		ZXString;
	typedef std::wstring	ZXWString;
	extern ZXWString ZXBase_API TranslateString(ZXString str);
	inline void string_replace(ZXString& strBig, const ZXString& strsrc, const ZXString& strdst)
	{
		ZXString::size_type pos=0;
		ZXString::size_type srclen=strsrc.size();
		ZXString::size_type dstlen=strdst.size();
		while( (pos=strBig.find(strsrc, pos)) != ZXString::npos){
			strBig.replace(pos, srclen, strdst);
			pos += dstlen;
		}
	}
}

#define DeclareProperty(kind, v_property)	\
	public:\
	const kind& get##v_property() const { return m_##v_property; };\
	kind& get##v_property(){ return m_##v_property; };\
	void set##v_property(kind value){ m_##v_property = value; };\
	protected:\
	kind m_##v_property
#define DeclarePtrProperty(kind, v_property)	\
	public:\
	kind get##v_property(){ return m_##v_property; };\
	void set##v_property(kind value){ m_##v_property = value; };\
	protected:\
	kind m_##v_property

#define Declare_List(class_name)\
	public:\
	typedef std::map<##class_name*,##class_name*> map##class_name;\
	static map##class_name& get_map_class_object(){ static map##class_name s_list_##class_name; return s_list_##class_name; }

#define Register_List_Variation(class_name)\
	get_map_class_object()[this]=this
#define UnRegister_List_Variation(class_name)\
	if(! get_map_class_object().empty()){\
	map##class_name::iterator itr =  get_map_class_object().find(this);\
	if(itr !=  get_map_class_object().end())  get_map_class_object().erase(itr);\
	}

#define MarcoBuffer(format, buffer)\
	va_list ptr;\
	va_start(ptr, ##format);\
	vsprintf_s(buffer,##format,ptr)

