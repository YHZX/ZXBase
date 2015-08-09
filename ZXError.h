#pragma once

#include "ZXBase/ZXItem.h"
#include <map>
namespace zx{

	class ZXBase_API ZXError : public ZXItem, public std::map<unsigned int, ZXString>
	{
	public:
		static ZXError* instance();
	public:
		DeclareZXBase(ZXError, ZXError);
		ZXError();
		void registerErrorCode(unsigned int code, ZXString describute);

		ZXString& getErrorDescribute(unsigned int code);
		unsigned int getLastError(){ return _lastError; };
		void setLastError(unsigned int e){ _lastError = e; };

		DeclareProperty(ZXString, ExternDescribute);
	protected:
		virtual ~ZXError();
	private:
		unsigned int	_lastError;
	};

#define ZXErrorInstance			zx::ZXError::instance
#define ZXSetLastError			ZXErrorInstance()->setLastError
#define ZXGetLastError			ZXErrorInstance()->getLastError
#define ZXGetErrorDescribute                ZXErrorInstance()->getErrorDescribute
#define ZXSetLastErrorExternDescribute			ZXErrorInstance()->setExternDescribute
#define ZXGetLastErrorExternDescribute			ZXErrorInstance()->getExternDescribute

#define ZXRegisterErrorCode		ZXErrorInstance()->registerErrorCode
};
